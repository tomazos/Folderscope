// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Memory.h"

#ifdef MY_DEBUG_MEMORY

class DebugMemory
{
public:
	static DebugMemory* instance();

	void* allocate(UInt32 iNumBytes);
	void deallocate(void* p);

private:
	DebugMemory();
	UInt8* m_pBuf;
	UInt32 m_iNextAlloc;
};

#endif
