// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

//#define MY_DEBUG_MEMORY

#define NewBytes(iNumBytes) ((void*) new char[iNumBytes])
#define DelBytes(pBytes) { delete [] ((char*) pBytes); }
#define TemplateInst(T, ...) T<__VA_ARGS__>
#define NewObject(T, ...) (new T (__VA_ARGS__))
#define DelObject(pObject) { delete pObject; }
#define NewArray(T, iNumElements) (new T [iNumElements])
#define DelArray(pArray) { delete [] pArray; }

#ifdef MY_DEBUG_MEMORY
void* operator new (size_t size);
void operator delete (void *p);
void* operator new[](size_t count);
void operator delete[] (void *p);
#endif

class Memory
{
public:
	static void zero(void* pBuf, Size iByteLength);

	static void copy(const void* pSource, void* pDestination, Size iNumBytes);
	static void move(const void* pSource, void* pDestination, Size iNumBytes);

	static int compare(const void* a, const void* b, Size iNumBytes);
	static inline bool equal(const void* a, const void* b, Size iNumBytes);
};

inline
bool Memory::equal(const void* a, const void* b, Size iNumBytes)
{
	return compare(a, b, iNumBytes) == 0;
}
