// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

class Lock
{
public:
	static CRITICAL_SECTION s_pGlobalLock;
	static void setupGlobalLock();

	inline Lock();
	inline Lock(const Lock&);
	inline Lock& operator=(const Lock&);
	inline ~Lock();

	inline void await();
	inline bool try_now();

	inline void release();

private:
	inline void initialize_();
	inline void delete_();

	CRITICAL_SECTION m_cs;
};

inline void Lock::initialize_()
{
	InitializeCriticalSection(&m_cs);
}

inline void Lock::delete_()
{
	DeleteCriticalSection(&m_cs);
}

inline Lock::Lock()
{
	initialize_();
}

inline Lock::Lock(const Lock&)
{
	initialize_();
}

inline Lock::~Lock()
{
	delete_();
}
inline Lock& Lock::operator=(const Lock&)
{
	delete_();
	initialize_();
	return (*this);
}

inline void Lock::await()
{
	EnterCriticalSection(&m_cs);
}

inline bool Lock::try_now()
{
	BOOL b = TryEnterCriticalSection(&m_cs);
	return !(b == 0);
}

inline void Lock::release()
{
	LeaveCriticalSection(&m_cs);
}
