// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/TimePeriod.h"

class TimePoint
{
public:
	inline TimePoint() {};
	inline TimePoint(const FILETIME& ft);

	static TimePoint now();
	static TimePoint foreverFuture();
	static TimePoint foreverPast();

	static TimePoint make(UInt16 iYear, UInt8 iMonth = 1, UInt8 iDay = 1,
		UInt8 iHour = 0, UInt8 iMinute = 0, Real64 fSeconds = 0);

	TimePeriod operator-(const TimePoint& that) const;

	TimePoint operator+(const TimePeriod& period) const;

	Ascii toString(const Chr8* fmt = "Y:M:D h:m:s.t") const; // t == microseconds

	static inline SInt64 comparef(const TimePoint& a, const TimePoint& b)
	{
		return TimePeriod::comparef(a.m_period, b.m_period);
	}

	inline bool operator== (const TimePoint& b) { return comparef((*this), b) == 0; }
	inline bool operator!= (const TimePoint& b) { return comparef((*this), b) != 0; }
	inline bool operator<  (const TimePoint& b) { return comparef((*this), b) < 0;  }
	inline bool operator>  (const TimePoint& b) { return comparef((*this), b) > 0;  }
	inline bool operator<= (const TimePoint& b) { return comparef((*this), b) <= 0; }
	inline bool operator>= (const TimePoint& b) { return comparef((*this), b) >= 0; }

private:
	TimePoint(const TimePeriod& timePeriod, UInt64 iTicks) 	: m_period(timePeriod), m_iTicks(iTicks) {}

	TimePeriod m_period; // since January 1, 1601 (UTC)
	UInt64 m_iTicks; // since processor reset

	static TimePeriod s_tickUnit;
	static bool s_bTickUnitSetup;
public:
	static void setupTickUnit();
};

inline TimePoint::TimePoint(const FILETIME& ft)
	: m_period(true, (UInt64(ft.dwHighDateTime) << 32) | UInt64(ft.dwLowDateTime), 0)
	, m_iTicks(UInt64(1) << 62)
{}

inline TimePoint operator-(const TimePoint& point, const TimePeriod& period) { return point + (-period); }

//commutative
inline TimePoint operator+(const TimePeriod& period, const TimePoint& point) { return point + period; }

inline UInt64 X86_RDTSC()
{
	UInt32 iLo;
	UInt32 iHi;

	__asm
	{
		push eax
		push edx
		
		rdtsc
		mov iLo, eax
		mov iHi, edx

		pop edx
		pop eax
	}

	return (UInt64(iHi) << 32) | UInt64(iLo);
};
