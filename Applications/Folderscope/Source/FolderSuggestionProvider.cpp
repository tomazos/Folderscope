// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderSuggestionProvider.h"

#include "TaskTracker.h"

#include "Stream.h"
#include "Runtime.h"

void FolderSuggestionProvider::search(const UTF16& sPrefix, int iSuggestionsRequired)
{
	Synchronizer sync(m_lock);
	m_sPrefix = sPrefix;
	m_iSuggestionsRequired = iSuggestionsRequired;
	m_bReset = true;
	m_work.open();
}

void FolderSuggestionProvider::shutdown()
{
	{
		Synchronizer sync(m_lock);
		m_bReset = true;
		m_bShutdown = true;
		m_work.open();
	}
	join();
}

static void notifySuggestions(FolderSuggestionProvider::Listener* p, HFolderSuggestionList h)
{
	p->notifySuggestions(h);
};

void FolderSuggestionProvider::run()
{
	while (true)
	{
		m_work.pass();

		m_lock.await();
		m_bReset = false;
		UTF16 sPrefix = m_sPrefix;
		int iSuggestionsRequired = m_iSuggestionsRequired;
		m_lock.release();
		
		HFolderSuggestionList hList = NewObject(FolderSuggestionList);
		listPathCompletions(sPrefix, (*hList), iSuggestionsRequired);

		Synchronizer sync(m_lock);

		if (m_bShutdown)
			break;

		if (m_bReset)
			continue;

		TreeSet<Listener*>::Iterator it(m_pListeners);
		Listener* pListener;

		while (it(pListener))
		{
			TaskTracker::instance()->add(buildFunctionTask(notifySuggestions, pListener, hList), TaskTracker::eMainThread);
		}
	}
}

void FolderSuggestionProvider::addListener(Listener* pListener)
{
	Synchronizer sync(m_lock);
	m_pListeners += pListener;
}

void FolderSuggestionProvider::removeListener(Listener* pListener)
{
	Synchronizer sync(m_lock);
	m_pListeners -= pListener;
}

static HUTF16List listLogicalDrives()
{
	HUTF16List hOutSubpaths = NewObject(UTF16List);
	Buffer buf(0);
	int iBufLength = GetLogicalDriveStrings(0, (LPTSTR) buf.ptr());
	if (iBufLength == 0)
		return hOutSubpaths;
	buf.resize(iBufLength*2);
	int iNumChrs = GetLogicalDriveStrings(buf.length() / 2, (LPTSTR) buf.ptr());

	const Chr16* pStart = ((Chr16*) buf.ptr());
	const Chr16* pEnd = pStart + iNumChrs;

	while (pStart < pEnd)
	{
		UTF16 s = UTF16(pStart);
		hOutSubpaths->add(s);
		pStart += s.length() + 1;
	}

	return hOutSubpaths;
}

static UTF16 getNotesOnLogicalDrive(Error& error, const UTF16& sLogicalDrive)
{
	UTF16Buffer sbVolName(MAX_PATH+1);
	UTF16Buffer sbFSType(MAX_PATH+1);

	if (!GetVolumeInformation(sLogicalDrive.ptr(), sbVolName.ptr(), 
		sbVolName.flength(), NULL, NULL, NULL, sbFSType.ptr(), sbFSType.flength()))
	{
		error.what(WindowsError::getLastError());
		error.who(L"GetVolumeInformation");
		return UTF16();
	}
	
	UTF16 sVolName = sbVolName;
	UTF16 sFSType = sbFSType;

	if (!sVolName.empty())
		sVolName = sVolName + " ";
	
	if (!sFSType.empty())
		sFSType = sFSType + " ";

	UTF16 sDriveType;

	switch (GetDriveType(sLogicalDrive.ptr()))
	{
	case DRIVE_NO_ROOT_DIR: sDriveType = L"No Root Dir "; break;
	case DRIVE_REMOVABLE: sDriveType = L"Removable "; break;
	case DRIVE_FIXED: sDriveType = L"Fixed "; break;
	case DRIVE_CDROM: sDriveType = L"CDROM "; break;
	case DRIVE_RAMDISK: sDriveType = L"RAM Disk "; break;
	default: sDriveType = L"Unknown ";
	}

	UTF16 sSizeInfo;

	UInt64 iBytesFree, iTotalBytes;
	ULARGE_INTEGER ulgFreeBytesAvailable, ulgTotalNumberOfBytes;
	if (GetDiskFreeSpaceEx(sLogicalDrive.ptr(), &ulgFreeBytesAvailable, &ulgTotalNumberOfBytes, NULL))
	{
		iBytesFree = ulgFreeBytesAvailable.QuadPart;
		iTotalBytes = ulgTotalNumberOfBytes.QuadPart;

		int iUnit = 0;
		for (UInt64 i = iTotalBytes; i > 0; i >>= 10)
			iUnit++;
		UTF16 sUnit;
		switch (iUnit)
		{
		case 0: iUnit = 1; sUnit = L"B"; break;
		case 1: sUnit = L"B"; break;
		case 2: sUnit = L"KB"; break;
		case 3: sUnit = L"MB"; break;
		case 4: sUnit = L"GB"; break;
		case 5: sUnit = L"TB"; break;
		case 6: sUnit = L"PB"; break;
		case 7: sUnit = L"EB"; break;
		case 8: sUnit = L"ZB"; break;
		case 9: sUnit = L"YB"; break;
		default: sUnit = L"??"; break;
		}

		Real64 fBytesFree = Real64(iBytesFree) / Real64(1ULL << (10*(iUnit-1)));
		Real64 fTotalBytes = Real64(iTotalBytes) / Real64(1ULL << (10*(iUnit-1)));
		Real64 fBytesUsed = fTotalBytes - fBytesFree;

		sSizeInfo = UTF16::format(L"%d/%d%s ", UInt32(fBytesUsed), UInt32(fTotalBytes), sUnit.ptr());
	}
	else
	{
		sSizeInfo = L"";
	}

	return UTF16::format(L"%s%s%s%s", sVolName.ptr(), sSizeInfo.ptr(), sDriveType.ptr(), sFSType.ptr());
}

void FolderSuggestionProvider::recursePathCompletions(int& iSuggestionsRemaining, FolderSuggestionList& suggestions)
{
	FolderSuggestionList::Iterator it(suggestions);
	FolderSuggestion sug;
	
	TreeSet<Path> processed;

	while (it(sug))
	{
		if (m_bReset)
		    return;

		processed += sug.path;
	}

	while (iSuggestionsRemaining > 0)
	{
		if (m_bReset)
		    return;

		if (!it(sug))
			return;

		UTF16 sPath = sug.path;

		Error error;

		Path path(error, sPath);

		if (error)
			continue;
		
		bool bIsDir = path.isDir(error);
		
		if (error)
			continue;

		if (!bIsDir)
			continue;

		HUTF16List subpaths = path.allsubs(error);

		UTF16List::Iterator it2(*subpaths);
		UTF16 subpath;
		while (it2(subpath))
		{
			if (m_bReset)
				return;

			Error error2;
			Path subdir = path.subpath(error2, subpath);

			if (error2)
				continue;

			bool bIsDir = subdir.isDir(error2);

			if (error2)
				continue;

			if (!bIsDir)
				continue;

			if (!processed[subdir])
			{
				processed += subdir;
				iSuggestionsRemaining--;
				FolderSuggestion sug2;
				sug2.path = subdir;
				sug2.notes = L"";
				suggestions.add(sug2);
			}
		}
	}
}

void FolderSuggestionProvider::listPathCompletions(const UTF16& sPrefix, FolderSuggestionList& suggestions, int iSuggestionsRequired)
{
	int iSuggestionsRemaining = iSuggestionsRequired;

	if (sPrefix.length() <= 3)
	{
		HUTF16List hLogicalDrives = listLogicalDrives();

		FolderSuggestion sug;
		UTF16List::Iterator it(*hLogicalDrives);
		UTF16 sLogicalDrive;
		while (it(sLogicalDrive))
		{
			Error error;

			sug.path = Path(error,sLogicalDrive);

			if (error)
				continue;

			if (m_bReset)
				return;

			if (sug.path.startsWith(sPrefix))
			{
				Error error;
				sug.notes = getNotesOnLogicalDrive(error, sug.path);
				if (!error)
				{
					iSuggestionsRemaining--;
					suggestions.add(sug);
				}
			}
		}

		if (!suggestions.empty())
		{
			recursePathCompletions(iSuggestionsRemaining, suggestions);
			return;
		}
	}

	UTF16 sLongestPrefixDir;
	for (sLongestPrefixDir = sPrefix; !sLongestPrefixDir.empty(); sLongestPrefixDir = sPrefix.prefix(sLongestPrefixDir.length() - 1))
	{
		if (m_bReset)
			return;

		Error error;
		Path path(error, sLongestPrefixDir);
		
		if (error)
			continue;

		bool bIsDir = path.isDir(error);

		if (error)
			continue;

		if (bIsDir)
			break;
	}

	if (sLongestPrefixDir.empty())
		return;

	Error error;
	Path longestPrefixDir(error, sLongestPrefixDir);

	if (error)
		return;

	//FolderSuggestion sug;
	//sug.path = sPrefix;
	//if (!Path::exists(sPrefix))
	//	sug.notes = L"(new)";
	//iSuggestionsRemaining--;
	//suggestions.add(sug);

	HUTF16List subpaths = longestPrefixDir.allsubs(error);

	if (error)
		return;

	UTF16List::Iterator it2(*subpaths);

	UTF16 sSubpath;
	while (it2(sSubpath))
	{
		if (m_bReset)
		    return;

		Error error2;

		Path subp = longestPrefixDir.subpath(error2, sSubpath);

		if (error2)
			continue;

		bool bIsDir = subp.isDir(error2);

		if (error2)
			continue;

		if (bIsDir && subp.startsWith(sPrefix))
		{
			FolderSuggestion sug2;
			sug2.path = subp;
			sug2.notes = L"";
			iSuggestionsRemaining--;
			suggestions.add(sug2);

			if (iSuggestionsRemaining == 0)
				return;
		}
	}

	recursePathCompletions(iSuggestionsRemaining, suggestions);
}
