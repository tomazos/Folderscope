// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Stream/InputStream.h"

class MemoryInputStream : public InputStream
{
public:
	MemoryInputStream(const void* pData, Size iNumBytes)
		: m_pEnd(((Byte*)pData) + iNumBytes)
		, m_iBytesRemaining(iNumBytes)
	{}

	virtual ~MemoryInputStream() {};

	virtual Size read(Error&, void* pBuf, Size iNumBytes);
private:

	Byte* m_pEnd;
	Size m_iBytesRemaining;

private: MemoryInputStream& operator=(const MemoryInputStream&) {};
};
