// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderImageImpl.h"

#include "FolderImageModeInitial.h"

#include "FolderImageThread.h"

#include "Runtime.h"

#include "FIIndex.h"
#include "FIContent.h"

void FolderImageImpl::addListener(Listener* pListener)
{
	Synchronizer sync(m_lock);

	m_listeners += pListener;
}

void FolderImageImpl::removeListener(Listener* pListener)
{
	Synchronizer sync(m_lock);

	m_listeners -= pListener;
}

void FolderImageImpl::FolderImage_enteringState(EState eState)
{
	notifyListeners(&Listener::FolderImage_enteringState, eState);
}

void FolderImageImpl::FolderImage_leavingState(EState eState)
{
	notifyListeners(&Listener::FolderImage_leavingState, eState);
}

void FolderImageImpl::FolderImage_progress(const ProgressReport& report)
{
	notifyListeners(&Listener::FolderImage_progress, report);
}

void FolderImageImpl::FolderImage_info(UTF16 sInfo)
{
	notifyListeners(&Listener::FolderImage_info, sInfo);
}

void FolderImageImpl::FolderImage_warning(UTF16 sError)
{
	notifyListeners(&Listener::FolderImage_warning, sError);
}

void FolderImageImpl::FolderImage_fatal(UTF16 sError)
{
	notifyListeners(&Listener::FolderImage_fatal, sError);
}

void FolderImageImpl::FolderImage_quickJobStart()
{
	notifyListeners(&Listener::FolderImage_quickJobStart);
}

void FolderImageImpl::FolderImage_quickJobEnd()
{
	notifyListeners(&Listener::FolderImage_quickJobEnd);
}

FolderImageImpl::FolderImageImpl()
	: m_eState(eInitial)
{
	m_mode = NewObject(FolderImageModeInitial, this);
	m_taskThread = NewObject(TaskThread);
	m_taskThread->start();
};

void FolderImageImpl::setRoot(const Path& root)
{
	m_db.m_root = root;
}

void FolderImageImpl::setLoadStream(HInputStream hLoadStream)
{
	m_hLoadStream = hLoadStream;
}

void FolderImageImpl::queueTask(HTask task)
{
	m_taskThread->add(task);
};

void FolderImageImpl::surfaceScan(bool& bComplete)
{
	if (m_surfaceScanner.isNull())
		m_surfaceScanner = NewObject(SurfaceScanner, *this, m_db.m_root);

	m_surfaceScanner->tick(bComplete);

	if (bComplete)
		m_surfaceScanner.setNull();
}

void FolderImageImpl::deepScan(bool& bComplete)
{
	if (m_deepScanner.isNull())
		m_deepScanner = NewObject(DeepScanner, *this, m_db.m_root, m_db);

	m_deepScanner->tick(bComplete);

	if (bComplete)
		m_deepScanner.setNull();
}

static UTF16 uppercaseString(const UTF16& s)
{
	UTF16Buffer sb(s.length()+1);
	for (int i = 0; i < s.length(); i++)
		sb[i] = Character::uppercase(s[i]);
	return sb;
}

static UTF16 parseFileExtension(const UTF16& sBasename)
{
	const Chr16* pStart = sBasename.ptr();
	const Chr16* p = pStart + sBasename.length();
	while (true)
	{
		if (p == pStart)
			return UTF16();

		p--;

		if (*p == L'.')
			return uppercaseString(UTF16(p+1));
	}
};


static FIItem::HComponents convertRelativeToComponents(const Array<UTF16>& relative)
{
	return NewObject(FIItem::Components, relative);
}

FIItem* FolderImageImpl::SurfaceScanner_startFolder(const Path& folder, Array<UTF16>& relative)
{
	FIDir* pParent = NULL;
	if (!m_pFIDirStack.empty())
		pParent = m_pFIDirStack.peek();

	FIDir* pDir = NewObject(FIDir, pParent, convertRelativeToComponents(relative));
	m_pFIDirStack.push(pDir);
	return pDir;
}

void FolderImageImpl::SurfaceScanner_skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage)
{
	logwarn("Skipping %s because %s", sAbsolutePath.ptr(), sErrorMessage.ptr());
}

void FolderImageImpl::SurfaceScanner_processFile(Error& error, const Path& file, const Array<UTF16>& relative)
{
	//HANDLE fh = CreateFile((UTF16("\\\\?\\") +file).ptr(), GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (fh == INVALID_HANDLE_VALUE)
	//{
	//	error.what(WindowsError::getLastError());
	//	error.who(L"CreateFile");
	//	return;
	//}

	//BY_HANDLE_FILE_INFORMATION fi;
	//Memory::zero(&fi, sizeof(fi));

	//BOOL res = GetFileInformationByHandle(fh,&fi);
	//if (res == 0)
	//{
	//	error.what(WindowsError::getLastError());
	//	error.who(L"GetFileInformationByHandle");
	//	return;
	//}

	//res = CloseHandle(fh);

	//Check(res != 0);

	//bool bAlreadyIn;
	//m_fileInfoSet.add(bAlreadyIn, fi.dwVolumeSerialNumber, fi.nFileIndexHigh, fi.nFileIndexLow);

	//if (bAlreadyIn)
	//{
	//	error.what(L"Already have it under different name");
	//	error.who(file.ptr());
	//	return;
	//}

	WinFindData wfd;
	WinFindFile wff((UTF16("\\\\?\\") +file).ptr());

	bool bFound = wff(error, wfd);

	if (error)
		return;

	if (!bFound)
	{
		Check();
		error.what(L"File not found");
		error.what(L"FindFile");
		return;
	}

	UInt64 iFileSize = wfd.getSize();
	TimePoint created = wfd.getCreationTime();
	TimePoint modified = wfd.getLastWriteTime();

	int iNumComponents = relative.size();

	FIItem::HComponents components = convertRelativeToComponents(relative);

	UTF16 sBasename = (*components)[iNumComponents - 1];
	UTF16 sExt = parseFileExtension(sBasename);

	FIFile* pFile = NewObject(FIFile, m_pFIDirStack.peek(), components, sExt, iFileSize, created, modified);
	m_db.m_pFIItems.push(pFile);
	m_db.m_pFIFiles.push(pFile);
}

void FolderImageImpl::SurfaceScanner_progress(const ProgressReport& report)
{
	FolderImage_progress(report);
}

void FolderImageImpl::SurfaceScanner_endFolder(FIItem* pItem) 
{
	if (m_pFIDirStack.empty())
	{
		logwarn("m_pFIDirStack.empty()");
		return;
	}

	FIDir* pDir = m_pFIDirStack.pop();

	if (pItem != pDir)
	{
		logwarn("pItem != m_pFIDirStack.peek()");
		return;
	}

	if (pDir->hasChildren())
	{
		m_db.m_pFIDirs.push(pDir);
		m_db.m_pFIItems.push(pDir);
	}
	else
	{
		pDir->detachFromParent();
		DelObject(pDir);
	}
}

void FolderImageImpl::DeepScanner_skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage)
{
	logwarn("Skipping %s because %s", sAbsolutePath.ptr(), sErrorMessage.ptr());
}
void FolderImageImpl::DeepScanner_progress(const ProgressReport& report)
{
	FolderImage_progress(report);
}

void FolderImageImpl::a_save(HOutputStream hOutputStream)
{
	m_mode->save(hOutputStream);
}

void FolderImageImpl::save(HOutputStream hOutputStream)
{
	deferTask(&FolderImageImpl::a_save, hOutputStream);
}

void FolderImageImpl::a_work()
{
    m_mode->work();
}

void FolderImageImpl::work()
{
    deferTask(&FolderImageImpl::a_work);
}

void FolderImageImpl::a_start()
{
    m_mode->start();
}

void FolderImageImpl::start()
{
    deferTask(&FolderImageImpl::a_start);
}

void FolderImageImpl::a_pause()
{
    m_mode->pause();
}

void FolderImageImpl::pause()
{
    deferTask(&FolderImageImpl::a_pause);
}

void FolderImageImpl::a_refresh()
{
    m_mode->refresh();
}

void FolderImageImpl::refresh()
{
    deferTask(&FolderImageImpl::a_refresh);
}

void FolderImageImpl::a_stop()
{
    m_mode->stop();
}

void FolderImageImpl::stop()
{
    deferTask(&FolderImageImpl::a_stop);
}

void FolderImageImpl::a_shutdown()
{
    m_mode->shutdown();
}

void FolderImageImpl::shutdown()
{
    deferTask(&FolderImageImpl::a_shutdown);
}

Size FolderImageImpl::getNoOfItems()
{
	return m_mode->getNoOfItems();
}

FIItem* FolderImageImpl::getItem(Size iIndex)
{
	return m_mode->getItem(iIndex);
}

void FolderImageImpl::sortStatus(FICriteria crit, int& iPriority, FICention& cent)
{
	m_comp.status(crit,iPriority, cent);
}

void FolderImageImpl::a_sort(FICriteria crit, FICention cent)
{
    m_mode->sort(crit, cent);
}

void FolderImageImpl::sort(FICriteria crit, FICention cent)
{
    deferTask(&FolderImageImpl::a_sort, crit, cent);
}

void FolderImageImpl::deepScan()
{
    deferTask(&FolderImageImpl::a_deepScan);
}

void FolderImageImpl::a_deepScan()
{
	m_mode->deepScan();
}

void FolderImageImpl::toggleMarkItem(Size iIndex)
{
	return m_mode->toggleMarkItem(iIndex);
}

void FolderImageImpl::a_markAll()
{
    m_mode->markAll();
}

void FolderImageImpl::markAll()
{
    deferTask(&FolderImageImpl::a_markAll);
}

void FolderImageImpl::a_markNone()
{
    m_mode->markNone();
}

void FolderImageImpl::markNone()
{
    deferTask(&FolderImageImpl::a_markNone);
}

void FolderImageImpl::a_markInvert()
{
    m_mode->markInvert();
}

void FolderImageImpl::markInvert()
{
    deferTask(&FolderImageImpl::a_markInvert);
}

void FolderImageImpl::a_markDuplicates()
{
    m_mode->markDuplicates();
}

void FolderImageImpl::markDuplicates()
{
    deferTask(&FolderImageImpl::a_markDuplicates);
}

void FolderImageImpl::a_markMatchPath()
{
    m_mode->markMatchPath();
}

void FolderImageImpl::markMatchPath()
{
    deferTask(&FolderImageImpl::a_markMatchPath);
}

void FolderImageImpl::a_markMatchContent()
{
    m_mode->markMatchContent();
}

void FolderImageImpl::markMatchContent()
{
    deferTask(&FolderImageImpl::a_markMatchContent);
}

void FolderImageImpl::a_deleteMarked()
{
    m_mode->deleteMarked();
}

void FolderImageImpl::deleteMarked()
{
    deferTask(&FolderImageImpl::a_deleteMarked);
}


void FolderImageImpl::a_renameMarked(UTF16 sSource, UTF16 sDestination)
{
	m_mode->renameMarked(sSource, sDestination);
}

void FolderImageImpl::renameMarked(UTF16 sSource, UTF16 sDestination)
{
	deferTask(&FolderImageImpl::a_renameMarked, sSource, sDestination);
}

void FolderImageImpl::a_copyMarked(UTF16 sSource, UTF16 sDestination)
{
	m_mode->copyMarked(sSource, sDestination);
}

void FolderImageImpl::copyMarked(UTF16 sSource, UTF16 sDestination)
{
	deferTask(&FolderImageImpl::a_copyMarked, sSource, sDestination);
}

void FolderImageImpl::a_externalCopy(UTF16 sAbsoluteExternalRoot)
{
	m_mode->externalCopy(sAbsoluteExternalRoot);
}

void FolderImageImpl::externalCopy(UTF16 sAbsoluteExternalRoot)
{
	deferTask(&FolderImageImpl::a_externalCopy, sAbsoluteExternalRoot);
}

void FolderImageImpl::a_externalMove(UTF16 sAbsoluteExternalRoot)
{
	m_mode->externalMove(sAbsoluteExternalRoot);
};

void FolderImageImpl::externalMove(UTF16 sAbsoluteExternalRoot)
{
	deferTask(&FolderImageImpl::a_externalMove, sAbsoluteExternalRoot);
};

bool FolderImageImpl::areChangesPending()
{
	return m_mode->areChangesPending();
}

Size FolderImageImpl::getNoOfChanges()
{
	return m_mode->getNoOfChanges();
}

FIChange* FolderImageImpl::getChange(Size iIndex)
{
	return m_mode->getChange(iIndex);
}

void FolderImageImpl::removeChange(FIChange* fiChange)
{
	return m_mode->removeChange(fiChange);
}

void FolderImageImpl::a_commitChanges()
{
    m_mode->commitChanges();
}

void FolderImageImpl::commitChanges()
{
    deferTask(&FolderImageImpl::a_commitChanges);
}


FolderImageImpl::~FolderImageImpl()
{
	m_taskThread->shutdown();
	DelObject(m_mode);
	for (int i = 0; i < m_db.m_pFIItems.size(); i++)
		DelObject(m_db.m_pFIItems[i]);
	for (int i = 0; i < m_db.m_pFIIndexes.size(); i++)
		DelObject(m_db.m_pFIIndexes[i]);
	for (int i = 0; i < m_db.m_pFIContents.size(); i++)
		DelObject(m_db.m_pFIContents[i]);
}
