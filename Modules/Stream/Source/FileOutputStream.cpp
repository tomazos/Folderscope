// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/FileOutputStream.h"

FileOutputStream::FileOutputStream(Error& error, const Path& path, FileOutputStream::EOpenType eOpenType)
{
	DWORD dwDesiredAccess = 0;
	switch (eOpenType)
	{
	case eCreate: dwDesiredAccess = CREATE_NEW; break;
	case eOverwrite: dwDesiredAccess = CREATE_ALWAYS; break;
	case eAppend: dwDesiredAccess = OPEN_ALWAYS; break;
	default: Check(); m_file = INVALID_HANDLE_VALUE; return;
	}

	m_file = CreateFile(path.ptr(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		dwDesiredAccess, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_file == INVALID_HANDLE_VALUE)
	{
		error.what(WindowsError::getLastError());
		error.who(L"CreateFile");
		return;
	}

	if (!CloseHandle(m_file))
	{
		error.what(WindowsError::getLastError());
		error.who(L"CloseHandle");
		m_file = INVALID_HANDLE_VALUE;
		return;
	}

	m_file = CreateFile(path.ptr(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_file == INVALID_HANDLE_VALUE)
	{
		error.what(WindowsError::getLastError());
		error.who(L"CreateFile");
		return;
	}

	if (SetFilePointer(m_file, 0, 0, FILE_END) == INVALID_SET_FILE_POINTER)
	{
		error.what(WindowsError::getLastError());
		error.who(L"SetFilePointer");
		return;
	}
}

FileOutputStream::~FileOutputStream()
{
	if (!CloseHandle(m_file))
		Check();
}

void FileOutputStream::write(Error& error, const void* pBuf, Size iNumBytes)
{
	DWORD iNumberOfBytesWritten;
	if (!WriteFile(m_file, pBuf, iNumBytes, &iNumberOfBytesWritten, NULL))
	{
		error.what(WindowsError::getLastError());
		error.who(L"WriteFile");
		return;
	}

	if (iNumberOfBytesWritten != iNumBytes)
	{
		error.what(L"Unable to write all data");
		error.what(L"FileOutputStream::write");
		return;
	}
}
