// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Blob.h"

#include "Basics/Memory.h"

Blob::Core::~Core()
{
	DelBytes(m_pContent);
	m_pContent = NULL;
}

void
Blob::setEmpty()
{
	Size iLength = 0;
	char* pBuf = (char*) NewBytes(0);

	m_core = NewObject(Core);
	m_core->m_iLength = iLength;
	m_core->m_pContent = pBuf;
}

Blob
Blob::own(void* p, Size iNumBytes)
{
	Handle<Core> core = NewObject(Core);
	core->m_iLength = iNumBytes;
	core->m_pContent = (Chr8*) p;
	return Blob(core);
}


Blob::Blob(const void* pContent, Size iNumBytes)
{
	if (pContent)
	{
		char* pBuf = (char*) NewBytes(iNumBytes);
		Memory::copy(pContent, pBuf, iNumBytes);

		m_core = NewObject(Core);
		m_core->m_iLength = iNumBytes;
		m_core->m_pContent = (Chr8*) pBuf;
	}
	else
	{
		setEmpty();
	}
}


bool
Blob::operator==(const Blob& that) const
{
	if (m_core->m_iLength != that.m_core->m_iLength)
		return false;

	int iLength = m_core->m_iLength;

	return Memory::equal(m_core->m_pContent, that.m_core->m_pContent, iLength);
}


UInt64 Blob::hashf(const Blob& x)
{
	UInt64 h = 0xA4238B58310421EFULL;

	UInt8* pStart = (UInt8*) x.ptr();
	UInt8* pEnd = pStart + x.length();

	for (UInt8* p = pStart; p != pEnd; p++)
		h += ((h << 5) ^ UInt64(*p));

	return h;
}

SInt64 Blob::comparef(const Blob& a, const Blob& b)
{
	UInt64 comp = Memory::compare(a.ptr(), b.ptr(), ((a.length() > b.length()) ? b.length() : a.length()));

	if (comp != 0)
		return comp;
	else
		return a.length() - b.length();
}
