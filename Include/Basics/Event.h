// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

class Event
{
public:
	Event();
	~Event();

	bool occured();

	void cause();

	void await();
	bool await(Size iMilliseconds);

	WindowsHandle getHandle() { return m_event; }

private:
	WindowsHandle m_event;
};
