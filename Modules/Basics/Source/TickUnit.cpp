// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/TickUnit.h"

#include "Basics/TimePoint.h"
#include "Basics/Synchronizer.h"

TickUnit::TickUnit()
{
	UInt64 iStartTicks = X86_RDTSC();

	Sleep(100);

	UInt64 iEndTicks = X86_RDTSC();

	m_tickUnit = TimePeriod::milliseconds(100) / Real64(iEndTicks - iStartTicks);
}

TickUnit* TickUnit::instance()
{
	GlobalSync;

	static TickUnit* p(NULL);
	if (!p)
		p = new TickUnit;

	return p;
}