// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Lock.h"

class Synchronizer
{
public:
	inline Synchronizer(Lock&);
	inline ~Synchronizer();
private:
	Lock& m_lock;
};

#define GlobalSync GlobalLockSynchronizer globalLockSync;

class GlobalLockSynchronizer
{
public:
	GlobalLockSynchronizer()
	{
		EnterCriticalSection(&Lock::s_pGlobalLock);
	}

	~GlobalLockSynchronizer()
	{
		LeaveCriticalSection(&Lock::s_pGlobalLock);
	}
};

inline Synchronizer::Synchronizer(Lock& lock)
	: m_lock(lock)
{
	m_lock.await();
}

inline Synchronizer::~Synchronizer()
{
	m_lock.release();
}
