// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/WinFindFile.h"

#include "Runtime/Log.h"

static inline UInt64 FILETIME_TO_UInt64(const FILETIME& ft)
{
	return (UInt64(ft.dwHighDateTime) << 32) | UInt64(ft.dwLowDateTime);
}

WinFindData::WinFindData()
{
	Memory::zero(&m_data, sizeof(m_data));
}

UTF16 WinFindData::getName()
{
	return UTF16(m_data.cFileName);
}

UInt64 WinFindData::getSize()
{
	return (UInt64(m_data.nFileSizeHigh) << 32) | UInt64(m_data.nFileSizeLow);
}

TimePoint WinFindData::getCreationTime()
{
	return TimePoint(m_data.ftCreationTime);
}

TimePoint WinFindData::getLastAccessTime()
{
	return TimePoint(m_data.ftLastAccessTime);
}

TimePoint WinFindData::getLastWriteTime()
{
	return TimePoint(m_data.ftLastWriteTime);
}

bool WinFindData::is_archive()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE);
}

bool WinFindData::is_compressed()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED);
}

bool WinFindData::is_device()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_DEVICE);
}

bool WinFindData::is_directory()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool WinFindData::is_encryptred()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED);
}

bool WinFindData::is_hidden()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
}

bool WinFindData::is_normal()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL);
}

bool WinFindData::is_not_context_indexed()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
}

bool WinFindData::is_offline()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE);
}

bool WinFindData::is_readonly()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
}

bool WinFindData::is_reparse_point()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT);
}

bool WinFindData::is_sparse_file()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE);
}

bool WinFindData::is_system()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM);
}

bool WinFindData::is_temporary()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY);
}

bool WinFindData::is_virtual()
{
	return 0 != (m_data.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL);
}

WinFindFile::WinFindFile(const UTF16& sPath)
	: m_sPath(sPath)
	, m_h(INVALID_HANDLE_VALUE)
{

}

bool WinFindFile::operator() (Error& error, WinFindData& wfd)
{
	if (m_h == INVALID_HANDLE_VALUE)
	{
		m_h = FindFirstFile(m_sPath.ptr(), &(wfd.m_data));
		if (m_h == INVALID_HANDLE_VALUE)
		{
			error.what(WindowsError::getLastError());
			error.who(L"FindFirstFile");
			return false;
		}
		else
			return true;
	}
	else
	{
		if (FindNextFile(m_h, &wfd.m_data))
			return true;

		int iLastError = GetLastError();

		if (iLastError == ERROR_NO_MORE_FILES)
			return false;
		
		error.what(WindowsError::codeToString(iLastError));
		error.who(L"FindNextFile");
		close();
		return false;
	}
}

void
WinFindFile::close()
{
	if (m_h != INVALID_HANDLE_VALUE)
		if (!FindClose(m_h))
			Check();

	m_h = INVALID_HANDLE_VALUE;
}

WinFindFile::~WinFindFile()
{
	close();
}
