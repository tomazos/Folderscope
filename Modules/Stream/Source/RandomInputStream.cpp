// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/RandomInputStream.h"

#include <Wincrypt.h>

RandomInputStream::RandomInputStream(Error& error)
{
	if (!CryptAcquireContext(&m_hProv,NULL, NULL, PROV_RSA_FULL, 0))
	{
		error.what(WindowsError::getLastError());
		error.who(L"CryptAcquiteContext");
	}
}

RandomInputStream::~RandomInputStream()
{
	if (!CryptReleaseContext(m_hProv, 0))
		Check();
}

Size RandomInputStream::read(Error& error, void* pBuf, Size iNumBytes)
{
	if (!CryptGenRandom(m_hProv,iNumBytes, (BYTE*) pBuf))
	{
		error.what(WindowsError::getLastError());
		error.who(L"CryptGenRandom");
		return 0;
	}

	return iNumBytes;
}
