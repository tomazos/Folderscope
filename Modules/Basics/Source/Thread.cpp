// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Thread.h"

#include "Basics/Check.h"

DWORD WINAPI Thread::threadProcStatic(LPVOID lpParameter)
{
	Thread* pThis = (Thread*) lpParameter;

	if (pThis->threadProcMember())
		return 0;
	else
		return 1;
}

bool Thread::threadProcMember()
{
	try {
		run();
		m_self = NULL;
		return true;
	}
	catch(...)
	{
		m_self = NULL;
		return false;
	}
}

Thread::Thread()
	: m_thread(NULL)
	, m_self(NULL)
{
	m_thread = CreateThread(NULL, 0, Thread::threadProcStatic, this, CREATE_SUSPENDED, NULL);
	Check(m_thread != NULL);
}

Thread::~Thread()
{
	BOOL bSucceeded = CloseHandle(m_thread);

	Check(bSucceeded != 0);
}

void
Thread::start()
{
	m_self = (isHandled() ? this : NULL);

	DWORD iRetVal = ResumeThread(m_thread);

	Check(iRetVal != -1);
}

void
Thread::join()
{
	DWORD iRes = WaitForSingleObject(m_thread, INFINITE);
	Check(iRes == WAIT_OBJECT_0);
}

void
Thread::sleep(int iMilliseconds)
{
	Sleep(iMilliseconds);
}
