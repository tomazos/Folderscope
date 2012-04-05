// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Turnstile.h"

#include "Basics/Check.h"

Turnstile::Turnstile()
	: m_event(CreateEvent(NULL, false, false, NULL))
{
	Check(m_event != NULL);
}

Turnstile::~Turnstile()
{
	BOOL bSucceeded = CloseHandle(m_event);

	Check(bSucceeded != 0);
}

void Turnstile::pass()
{
	DWORD iRes = WaitForSingleObject(m_event, INFINITE);

	Check(iRes == WAIT_OBJECT_0);
}

bool Turnstile::pass(int iMilliseconds)
{
	DWORD iRes = WaitForSingleObject(m_event, iMilliseconds);

	if (iRes == WAIT_OBJECT_0)
	{
		return true;
	}
	else
	{
		Check(iRes == WAIT_TIMEOUT);
		return false;
	}
}

void
Turnstile::open()
{
	BOOL bSucceeded = SetEvent(m_event);

	Check(bSucceeded != 0);
}
