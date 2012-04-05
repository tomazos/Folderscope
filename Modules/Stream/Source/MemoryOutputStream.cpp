// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/MemoryOutputStream.h"

MemoryOutputStream::MemoryOutputStream(Buffer& buf)
	: m_buf(buf)
{}

void MemoryOutputStream::write(Error&, const void* pBuf, Size iNumBytes)
{
	Size iOldLength = m_buf.length();
	m_buf.resize(iOldLength + iNumBytes);
	Memory::copy(pBuf, &(m_buf.idx<Byte>(iOldLength)), iNumBytes);
}
