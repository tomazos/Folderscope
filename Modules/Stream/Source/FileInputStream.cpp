// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/FileInputStream.h"

#include "Stream/Path.h"

FileInputStream::FileInputStream(Error& error, const Path& path)
{
	m_file = CreateFile(path.ptr(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (m_file == INVALID_HANDLE_VALUE)
	{
		error.what(WindowsError::getLastError());
		error.who(L"CreateFile");
	}
}

FileInputStream::~FileInputStream()
{
	if (m_file != INVALID_HANDLE_VALUE)
	{
		if (!CloseHandle(m_file))
			Check();
	}
}

Size FileInputStream::read(Error& error, void* pBuf, Size iNumBytes)
{
	DWORD iNumberOfBytesRead;
	if (!ReadFile(m_file, pBuf, iNumBytes, &iNumberOfBytesRead, NULL))
	{
		error.what(WindowsError::getLastError());
		error.who(L"ReadFile");
	}
	return iNumberOfBytesRead;
}

void FileInputStream::getFileIndexInfo(Error& error, UInt32& iVolumeIndex, UInt32& iFileIndexHi, UInt32& iFileIndexLo)
{
	BY_HANDLE_FILE_INFORMATION fi;
	BOOL bSuccess = GetFileInformationByHandle(m_file, &fi);
	if (bSuccess == 0)
	{
		error.what(WindowsError::getLastError());
		error.who(L"GetFileInformationByHandle");
		return;
	}
	iVolumeIndex = fi.dwVolumeSerialNumber;
	iFileIndexHi = fi.nFileIndexHigh;
	iFileIndexLo = fi.nFileIndexLow;
}
