// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/DebugMemory.h"

#ifdef MY_DEBUG_MEMORY

#include "Basics/Synchronizer.h"
#include "Basics/Check.h"
#include "Basics/Memory.h"

#include <cstring>
#include <cstdio>

FILE* g_dumpFile = NULL;

void vdumpf(const Chr8* sFormat, va_list args)
{
	std::vfprintf(g_dumpFile, sFormat, args);
	fflush(g_dumpFile);
}

void dumpf(const Chr8* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);
	vdumpf(sFormat, args);
	va_end(args);
}

DebugMemory* DebugMemory::instance()
{
	GlobalSync;

	static DebugMemory* p(NULL);
	if (!p)
	{
		void* vp = malloc(sizeof(DebugMemory));
		p = new (vp) DebugMemory;
	}

	return p;
}

const int g_iHeapSize = 1 << 29;

void mempat(UInt8* pBuf, UInt32 iPat, UInt32 iNumBytes)
{
	if (iNumBytes == 0)
		return;

	Check(UInt32(pBuf) % 4 == 0);
	UInt32 iBytesMarked = 0;

	pBuf[0] = (iPat >> 24) & 0xFF;
	iBytesMarked++; if (iNumBytes == iBytesMarked) return;

	pBuf[1] = (iPat >> 16) & 0xFF;
	iBytesMarked++; if (iNumBytes == iBytesMarked) return;

	pBuf[2] = (iPat >>  8) & 0xFF;
	iBytesMarked++; if (iNumBytes == iBytesMarked) return;

	pBuf[3] = (iPat >>  0) & 0xFF;
	iBytesMarked++; if (iNumBytes == iBytesMarked) return;

	while (iBytesMarked != iNumBytes)
	{
		UInt32 iBytesToMark = iBytesMarked;
		if (iNumBytes < iBytesMarked + iBytesToMark)
			iBytesToMark = iNumBytes - iBytesMarked;

		Memory::copy(pBuf, pBuf+iBytesMarked, iBytesToMark);
		iBytesMarked += iBytesToMark;
	}
}

DebugMemory::DebugMemory()
{
	if (g_dumpFile == NULL)
	{
		g_dumpFile = fopen("C:\\Folderscope_Memory_Dump.txt", "w");
		Check(g_dumpFile != NULL);
	}

	m_pBuf = (UInt8*) (malloc(g_iHeapSize));
	dumpf("Created heap at: %p <--> %p\n", m_pBuf, m_pBuf + g_iHeapSize);
	Check(m_pBuf != NULL);
	
	mempat(m_pBuf, 0xDEADBEEFU, g_iHeapSize);
	m_iNextAlloc = 8;
}

void* DebugMemory::allocate(UInt32 iNumBytes)
{
	UInt32 im4 = iNumBytes % 4;
	if (im4 != 0)
		iNumBytes += (4 - im4);

	if (m_iNextAlloc + iNumBytes > g_iHeapSize)
		return NULL;
		
	void* p = m_pBuf + m_iNextAlloc;

	m_iNextAlloc += iNumBytes + 8;

//	dumpf("%p : allocate(%u)\n", p, iNumBytes);

	Check(UInt32(p) % 4 == 0);
	return p;
}

void DebugMemory::deallocate(void* p)
{
//	dumpf("%p : deallocate\n", p);
}

#endif