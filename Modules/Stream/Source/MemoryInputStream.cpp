// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/MemoryInputStream.h"

Size MemoryInputStream::read(Error&, void* pBuf, Size iNumBytes)
{
	Size iBytesToRead = ((iNumBytes <= m_iBytesRemaining) ? iNumBytes : m_iBytesRemaining);
	Memory::copy(m_pEnd - m_iBytesRemaining, pBuf, iBytesToRead);
	m_iBytesRemaining -= iBytesToRead;
	return iBytesToRead;
}