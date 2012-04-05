// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Handle.h"
#include "Basics/Handleable.h"
#include "Basics/WeakHandle.h"
#include "Basics/WeakHandleable.h"

DeclWeakHandleable(Thread);
class Thread : public WeakHandleable
{
public:
	Thread();
	virtual ~Thread();

	void start();
	void join();

	static void sleep(int iMilliseconds);

protected:

	virtual void run() = 0;

private:
	WindowsHandle m_thread;

	HThread m_self;
	Thread(const Thread&);
	Thread& operator=(const Thread&);

	static DWORD WINAPI threadProcStatic(LPVOID lpParameter);
	bool threadProcMember();
};
