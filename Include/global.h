#pragma once

#include <new>

#include "platform.h"

#ifndef NULL
	#define NULL 0
#endif

typedef char Byte;
typedef int Size;

typedef                char    SInt8;
typedef unsigned       char    UInt8;

typedef          short  int    SInt16;
typedef unsigned short  int    UInt16;

typedef                 int    SInt32;
typedef unsigned        int    UInt32;

typedef           long long    SInt64;
typedef unsigned  long long    UInt64;

typedef float				   Real32;
typedef double				   Real64;

typedef                char   Chr8;

#ifdef OS_WINDOWS
typedef             wchar_t   Chr16;
#endif
#ifdef OS_UNIX
typedef             UInt16    Chr16;
#endif

#ifdef OS_WINDOWS
typedef             UInt32   Chr32;
#endif
#ifdef OS_UNIX
typedef            wchar_t   Chr32;
#endif

#ifdef OS_WINDOWS
	
	#ifndef UNICODE
		#define UNICODE
	#endif

	#ifndef  WIN32_LEAN_AND_MEAN
		#define  WIN32_LEAN_AND_MEAN
	#endif

	#include <windows.h>
	typedef HANDLE WindowsHandle;

#endif

