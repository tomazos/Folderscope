// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/TimePeriod.h"

class TickUnit
{
public:
	static TickUnit* instance();

	TimePeriod get() { return m_tickUnit; }

private:
	TimePeriod m_tickUnit;

	TickUnit();
};
