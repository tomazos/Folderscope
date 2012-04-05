// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/WindowsError.h"

UTF16 WindowsError::getLastError()
{
	int iLastError = GetLastError();
	return codeToString(iLastError);
}

UTF16 WindowsError::codeToString(int iErrorCode)
{
	Chr16* pBuf = NULL;

    if (0 != FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        iErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &pBuf,
        0, NULL ))
	{
		UTF16 s(pBuf);
		LocalFree(pBuf);
		return s;
	}
	else
		return UTF16::format(L"Unknown windows error #%d.", iErrorCode);
}

