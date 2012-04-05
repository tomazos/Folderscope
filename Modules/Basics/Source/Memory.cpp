// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Memory.h"
#include "Basics/Lock.h"
#include "Basics/Synchronizer.h"
#include "Basics/Check.h"

#include <typeinfo.h>

#include <cstdlib>
#include <cstring>

#include <cstdio>

#include "Basics/Handleable.h"

#include "Basics/DebugMemory.h"

void Memory::copy(const void* pSource, void* pDestination, Size iNumBytes)
{
	std::memcpy(pDestination, pSource, iNumBytes);
}

void Memory::move(const void* pSource, void* pDestination, Size iNumBytes)
{
	MoveMemory(pDestination, pSource, iNumBytes);
}

int Memory::compare(const void* a, const void* b, Size iNumBytes)
{
	return std::memcmp(a,b,iNumBytes);
}

void Memory::zero(void* pBuf, Size iByteLength)
{
	SecureZeroMemory(pBuf, iByteLength);
}

#ifdef MY_DEBUG_MEMORY

void* operator new (size_t size)
{
	return DebugMemory::instance()->allocate(size);
}

void operator delete (void *p)
{
	return DebugMemory::instance()->deallocate(p);
}

void* operator new[](size_t count)
{
	return operator new(count);
}

void operator delete[] (void *p)
{
	operator delete(p);
}

#endif
