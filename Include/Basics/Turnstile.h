// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

class Turnstile
{
public:
	Turnstile();
	~Turnstile();

	void open();

	void pass();
	bool pass(int iMilliseconds);

	WindowsHandle getHandle() { return m_event; }

private:
	WindowsHandle m_event;
};
