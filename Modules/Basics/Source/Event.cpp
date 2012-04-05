// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Event.h"

#include "Basics/Check.h"

Event::Event()
	: m_event(CreateEvent(NULL, true, false, NULL))
{
	if (!m_event)
	{
		Check();
	}
}

Event::~Event()
{
	BOOL bSucceeded = CloseHandle(m_event);

	Check(bSucceeded != 0);
}

void Event::await()
{
	DWORD iRes = WaitForSingleObject(m_event, INFINITE);

	Check(iRes == WAIT_OBJECT_0);
}

bool Event::await(int iMilliseconds)
{
	DWORD iRes = WaitForSingleObject(m_event, iMilliseconds);

	if (iRes == WAIT_OBJECT_0)
		return true;
	else if (iRes == WAIT_TIMEOUT)
		return false;
	else
	{
		Check();
		return false;
	}
}

void
Event::cause()
{
	BOOL bResult = SetEvent(m_event);
	Check(bResult != 0);
}

bool
Event::occured()
{
	return await(0);
}
