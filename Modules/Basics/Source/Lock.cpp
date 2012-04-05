// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Lock.h"

CRITICAL_SECTION Lock::s_pGlobalLock;

void Lock::setupGlobalLock()
{
	InitializeCriticalSection(&s_pGlobalLock);
}
