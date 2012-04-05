// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/Path.h"

#include "Stream/FileInputStream.h"
#include "Stream/FileOutputStream.h"
#include "Stream/RandomInputStream.h"
#include "Stream/WinFindFile.h"

#include <shlobj.h>

static bool isSeparator(Chr16 c)
{
	return c == L'\\' || c == L'/';
}

bool Path::invalid_(Error& error) const
{
	if (!m_bValid)
	{
		error.what(L"Invalid path");
		error.who(L"Path");
		return true;
	}

	return false;
}

Path Path::getCurrentDir(Error& error)
{
	Size iCurrentDirSize = GetCurrentDirectory(0,NULL);

	if (iCurrentDirSize <= 0)
	{
		error.what(WindowsError::getLastError());
		error.who(L"GetCurrentDirectory");
		return Path();
	}
	
	UTF16Buffer sbCurrentDirectory(iCurrentDirSize+2);

	Size iActualCurrentDirSize = GetCurrentDirectory(iCurrentDirSize + 2, sbCurrentDirectory.ptr());

	if (iActualCurrentDirSize <= 0)
	{
		error.what(WindowsError::getLastError());
		error.who(L"GetCurrentDirectory");
		return Path();
	}

	return Path(error, sbCurrentDirectory);
}

void Path::setCurrentDir(Error& error, const Path& path)
{
	BOOL bSuccess = SetCurrentDirectory(path.ptr());
	if (bSuccess == 0)
	{
		error.what(WindowsError::getLastError());
		error.who(L"SetCurrentDirectory");
	}
}

Path Path::createTempDir(Error& error, const UTF16& sPrefix)
{
	int iTempPathLength = GetTempPath(0, NULL);
	
	if (iTempPathLength == 0)
	{
		error.what(WindowsError::getLastError());
		error.who(L"GetTempPath");
		return Path();
	}

	UTF16Buffer sbTempPath(iTempPathLength);

	DWORD iRes = GetTempPath(sbTempPath.flength(), sbTempPath.ptr());

	if (iRes == NULL)
	{
		error.what(WindowsError::getLastError());
		error.who(L"GetTempPath");
		return Path();
	}

	UTF16 sTempPath = sbTempPath;

	Path tempDir(error, sTempPath);

	if (error)
		return Path();

	RandomInputStream random(error);

	if (error)
		return Path();

	Buffer randBuf(24);

	if (random.read(error, randBuf.ptr(),24) != 24)
	{
		if (error)
			return Path();

		error.what(L"RandomInputStream::read failed");
		return Path();
	}

	if (error)
		return Path();

	Blob randBlob = randBuf;

	Path newTempDir = tempDir.subpath(error, sPrefix + L"." + HexString::binToHex(randBlob));

	if (error)
		return Path();

	newTempDir.createDir(error);

	if (error)
		return Path();

	return newTempDir;
}


Path Path::getAppDataFolder(Error& error)
{
	UTF16Buffer sbAppDataPath(MAX_PATH+1);

	if (S_OK == SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, sbAppDataPath.ptr()))
	{
		UTF16 sAppDataPath = sbAppDataPath;
		
		Path appDataPath(error, sAppDataPath);

		if (error)
			return Path();

		if (appDataPath.isDir(error))
		{
			if (error)
				return Path();

			return Path(error, appDataPath);
		}

		if (error)
			return Path();

		appDataPath.vivDirForce(error);

		if (error)
			return Path();

		return appDataPath;
	}
	else
	{
		error.what(L"SHGetFolderPath != S_OK");
		return Path();
	}
}

Path::Path()
	: m_bValid(false)
{

}

static bool isAbsolutePath(const UTF16& sPath)
{
	bool bIsAbsolutePath = true;

	bIsAbsolutePath = bIsAbsolutePath && sPath.length() >= 3;
	bIsAbsolutePath = bIsAbsolutePath && Character::isAlpha(sPath[0]);
	bIsAbsolutePath = bIsAbsolutePath && sPath[1] == L':';
	bIsAbsolutePath = bIsAbsolutePath && isSeparator(sPath[2]);

	return bIsAbsolutePath;
}

static void getFullPathName_(Error& error, const UTF16& sRawPath, UTF16& sNormalizedPath)
{
	Size iFullPathSize = GetFullPathName(sRawPath.ptr(), 0, NULL, NULL);

	if (iFullPathSize <= 0)
	{
		error.what(WindowsError::getLastError());
		error.who(L"GetFullPathName");
		sNormalizedPath = sRawPath;
		return;
	}

	UTF16Buffer sbNormalizedPath(iFullPathSize);

	DWORD iRes = GetFullPathName(sRawPath.ptr(), iFullPathSize, sbNormalizedPath.ptr(), NULL);

	if (iRes == 0)
	{
		error.what(WindowsError::getLastError());
		error.who(L"GetFullPathName");
		sNormalizedPath = sRawPath;
		return;
	}

	sNormalizedPath = sbNormalizedPath;
}

void Path::parsePath(Error& error, const UTF16& sRawPath)
{
	if (!isAbsolutePath(sRawPath))
	{
		error.what(L"Malformed path");
		m_sPath = sRawPath;
		return;
	}

	UTF16 sNormalizedPath;

	getFullPathName_(error, sRawPath, m_sPath);
	
	if (error)
		return;

	while (m_sPath.length() > 3 && isSeparator(m_sPath[m_sPath.length() - 1]))
		m_sPath = m_sPath.prefix(m_sPath.length() - 1);
}

Path::Path(Error& error, const UTF16& sPath)
	: m_bValid(true)
{
	parsePath(error, sPath);
	if (error)
		m_bValid = false;
}

Path::operator UTF16() const
{
	return m_sPath;
}

const Chr16* Path::ptr() const
{
	return m_sPath.ptr();
}

SInt64 Path::comparef(const Path& a, const Path& b)
{
	return compareUTF16CaseInsensetive(a,b);
}

bool Path::startsWith(const UTF16& sPrefix)
{
	return 0 == compareUTF16CaseInsensetive(sPrefix, m_sPath.prefix(sPrefix.length()));
}

bool Path::endsWith(const UTF16& sSuffix)
{
	return 0 == compareUTF16CaseInsensetive(sSuffix, m_sPath.suffix(sSuffix.length()));
}

bool Path::contains(const UTF16& sSubstring)
{
	if (sSubstring.empty())
		return true;

	int iStartMax = m_sPath.length() - sSubstring.length();

	for (int i = 0; i <= iStartMax; i++)
		if (0 == compareUTF16CaseInsensetive(sSubstring, m_sPath.substring(i, sSubstring.length())))
			return true;

	return false;
}

bool Path::operator== (const Path& path) const
{
	return comparef(*this, path) == 0;
}

bool Path::operator!= (const Path& path) const
{
	return comparef(*this, path) != 0;
}

bool Path::operator< (const Path& path) const
{
	return comparef(*this, path) > 0;
}

bool Path::operator> (const Path& path) const
{
	return comparef(*this, path) < 0;
}

bool Path::operator<= (const Path& path) const
{
	return comparef(*this, path) >= 0;
}
	
bool Path::operator>= (const Path& path) const
{
	return comparef(*this, path) <= 0;
}

bool Path::isValid() const
{
	return m_bValid;
}
	
bool Path::exists(Error& error) const
{
	if (invalid_(error)) return false;

	int attributes = GetFileAttributes(ptr());

	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		DWORD iError = GetLastError();
		switch (iError)
		{
		case ERROR_FILE_NOT_FOUND: return false;
		case ERROR_PATH_NOT_FOUND: return false;
		default:
			{
				error.what(WindowsError::getLastError());
				error.who(L"GetFileAttributes");
				return false;
			}
		}
	}
	return true;
}

Path Path::parent(Error& error) const
{
	if (invalid_(error)) return Path();

	Check(m_sPath.length() >= 3);

	if (m_sPath.length() == 3)
	{
		error.what(L"Root path has no parent");
		return Path();
	}

	const Chr16* pStart = m_sPath.ptr();
	const Chr16* pEnd = pStart + m_sPath.length();

	const Chr16* p = pEnd;

	if (p == pStart)
	{
		error.what(L"Has no parent");
		return Path();
	}

	p--;

	// skip trailing seperators
	while (isSeparator(*p))
	{
		if (p == pStart)
		{
			error.what(L"Has no parent");
			return Path();
		}

		p--;
	}

	do
	{
		if (p == pStart)
		{
			error.what(L"Has no parent");
			return Path();
		}

		p--;
	}
	while (!isSeparator(*p));

	UTF16Buffer sbParentPath((p-pStart) + 2);

	Memory::copy(pStart, sbParentPath.ptr(), 2*(p-pStart+1));
	
	return Path(error, UTF16(sbParentPath));	
}

bool Path::isFile(Error& error) const
{
	if (invalid_(error)) return false;

	int attributes = GetFileAttributes(ptr());

	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		DWORD iError = GetLastError();
		switch (iError)
		{
		case ERROR_FILE_NOT_FOUND: return false;
		case ERROR_PATH_NOT_FOUND: return false;
		default:
			{
				error.what(WindowsError::getLastError());
				error.who(L"GetFileAttributes");
				return false;
			}
		}
	}
	return (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;	
}

bool Path::isDir(Error& error) const
{
	if (invalid_(error)) return false;

	int attributes = GetFileAttributes(ptr());

	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		DWORD iError = GetLastError();
		switch (iError)
		{
		case ERROR_FILE_NOT_FOUND: return false;
		case ERROR_PATH_NOT_FOUND: return false;
		default:
			{
				error.what(WindowsError::getLastError());
				error.who(L"GetFileAttributes");
				return false;
			}
		}
	}
	return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool Path::isRootDir(Error& error) const
{
	if (invalid_(error)) return false;

	bool bResult = m_bValid && m_sPath.length() == 3 && isDir(error);

	if (error)
		return false;

	return bResult;
}

bool Path::isEmptyDir(Error& error) const
{
	if (invalid_(error)) return false;

	if (!isDir(error)) return false;

	WinFindFile wff(m_sPath + L"\\*");
	WinFindData wfd;

	while (wff(error, wfd))
	{
		if (error)
			return false;

		if (wfd.getName() == L"." || wfd.getName() == L"..")
			continue;

		return false;
	}

	return true;
}

Path Path::subpath(Error& error, const UTF16& sRelativePath) const
{
	if (invalid_(error)) return Path();

	UTF16 sPath = m_sPath;

	if (!(sPath.endsWith(L"\\") || sPath.endsWith(L"/")))
		sPath = sPath + L"\\";

	sPath = sPath + sRelativePath;

	return Path(error, sPath);
}

HUTF16List Path::allsubs(Error& error) const
{
	HUTF16List hOutSubpaths = NewObject(UTF16List);

	if (invalid_(error)) return hOutSubpaths;

	WinFindFile wff(m_sPath + L"\\*");

	WinFindData wfd;
	while (wff(error, wfd))
	{
		UTF16 sName = wfd.getName();

		if (sName == L"." || sName == L".." || wfd.is_reparse_point())
			continue;

		hOutSubpaths->add(sName);
	}

	return hOutSubpaths;
}

bool Path::walk(PathWalker& walker) const
{
	bool bStop = false;

	Error error;

	if (invalid_(error))
	{
		walker.onError(*this, error, bStop);
		return bStop;
	}

	bool bIsFile = isFile(error);
	if (error)
	{
		walker.onError(*this, error, bStop);
		return bStop;
	}

	if (bIsFile)
	{
		walker.processFile(*this, bStop);
		return bStop;
	}

	bool bIsDir = isDir(error);
	if (error)
	{
		walker.onError(*this, error, bStop);
		return bStop;
	}
	if (!bIsDir)
	{
		error.what(L"Not a dir or file");
		walker.onError(*this, error, bStop);
		return bStop;
	}

	HUTF16List subpaths = allsubs(error);

	if (error)
	{
		walker.onError(*this, error, bStop);
		return bStop;
	}

	walker.startDir(*this, bStop);

	if (bStop)
		return bStop;

	UTF16List::Iterator it(*subpaths); UTF16 sSubPath;

	while (it(sSubPath))
	{
		Path p = subpath(error, sSubPath);

		if (error)
		{
			walker.onError(*this, error, bStop);
			if (bStop)
				return bStop;
			else
				continue;
		}

		bStop = p.walk(walker);

		if (bStop)
			return bStop;
    }

	walker.endDir(*this, bStop);

	return bStop;
}

void Path::createDir(Error& error) const
{
	if (invalid_(error)) return;

	if (!CreateDirectory(ptr(), NULL))
	{
		error.what(WindowsError::getLastError());
		error.who(L"CreateDirectory");
	}
}

void Path::vivDir(Error& error) const
{
	if (invalid_(error)) return;

	bool bIsDir = isDir(error);

	if (error)
		return;

	if (!bIsDir)
		createDir(error);
}

void Path::vivDirForce(Error& error) const
{
	if (invalid_(error)) return;

	Path p = (*this);

	Array<Path> dirsToViv;

	while (true)
	{
		bool bIsRootDir = p.isRootDir(error);
		
		if (error)
			return;

		if (bIsRootDir)
			break;
	
		dirsToViv.push(p);
		
		p = p.parent(error);
		
		if (error)
			return;
	}

	while (!dirsToViv.empty())
	{
		dirsToViv.pop().vivDir(error);

		if (error)
			return;
	}
}

void Path::deleteDirEmpty(Error& error) const
{
	if (invalid_(error)) return;

	if (!RemoveDirectoryW(ptr()))
	{
		error.what(WindowsError::getLastError());
		error.who(L"RemoveDirectory");
	}
}

class DirDeleter : public PathWalker
{
public:
	Error& m_error;
	DirDeleter(Error& error)
		: m_error(error)
	{}

	virtual void processFile(const Path& p, bool& bStop)
	{
		p.deleteFile(m_error);

		if (m_error)
			bStop = true;
	}

	virtual void endDir(const Path& p, bool& bStop)
	{
		p.deleteDirEmpty(m_error);

		if (m_error)
			bStop = true;
	}

	virtual void onError(const Path&, const Error& error, bool& bStop)
	{
		m_error = error;
		bStop = true;
	}
};

void Path::deleteDirForce(Error& error) const
{
	if (invalid_(error)) return;

	DirDeleter deleter(error);
	walk(deleter);
}

UInt64 Path::fileSize(Error& error) const
{
	if (invalid_(error)) return 0;

	WinFindFile wff(m_sPath);
	WinFindData wfd;

	bool bFound = wff(error, wfd);

	if (error)
		return 0;

	if (!bFound)
	{
		error.what(L"No such file");
		return 0;
	}

	if (!isFile(error))
	{
		if (error)
			return 0;

		error.what(L"Not a file");
		return 0;
	}

	if (error)
		return 0;

	return wfd.getSize();
}

void Path::readFile(Error& error, HInputStream& is) const
{
	if (invalid_(error)) return;

	is = NewObject(FileInputStream, error, *this);
}

void Path::slurpFile(Error& error, Blob& b) const
{
	if (invalid_(error)) return;

	HInputStream is;
	readFile(error, is);
	if (error)
		return;
	b = is->slurp(error);
}

void Path::createFile(Error& error, HOutputStream& os) const
{
	if (invalid_(error)) return;

	os = NewObject(FileOutputStream, error, *this, FileOutputStream::eCreate);
}

void Path::overwriteFile(Error& error, HOutputStream& os) const
{
	if (invalid_(error)) return;

	os = NewObject(FileOutputStream, error, *this, FileOutputStream::eOverwrite);
}

void Path::appendFile(Error& error, HOutputStream& os) const
{
	if (invalid_(error)) return;

	os = NewObject(FileOutputStream, error, *this, FileOutputStream::eAppend);
}

void Path::deleteFile(Error& error) const
{
	if (invalid_(error)) return;

	if (!DeleteFile(ptr()))
	{
		error.what(WindowsError::getLastError());
		error.who(L"DeleteFile");
	}
}

UTF16 Path::basename() const
{
	if (!m_bValid)
	{
		Check();
		return m_sPath;
	}

	Error error;

	Path parent_ = parent(error);
	
	if (error || !parent_.isValid())
		return m_sPath;

	return m_sPath.suffix(m_sPath.length() - UTF16(parent_).length());
}
