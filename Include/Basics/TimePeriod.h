// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Ascii.h"

class TimePeriod
{
public:
	static inline TimePeriod notime();
	static inline TimePeriod foreverFuture();
	static inline TimePeriod foreverPast();

	static inline TimePeriod years(Real64);
	static inline TimePeriod months(Real64);
	static inline TimePeriod weeks(Real64);
	static inline TimePeriod days(Real64);
	static inline TimePeriod hours(Real64);
	static inline TimePeriod minutes(Real64);
	static TimePeriod seconds(Real64);
	static inline TimePeriod milliseconds(Real64);
	static inline TimePeriod microseconds(Real64);
	static inline TimePeriod nanoseconds(Real64);
	static inline TimePeriod picoseconds(Real64);

	TimePeriod operator+(const TimePeriod&) const;
	inline TimePeriod operator-() const;
	TimePeriod operator*(Real64) const;
	Real64 operator/(const TimePeriod&) const;
	static SInt64 comparef(const TimePeriod& a, const TimePeriod& b);

	inline TimePeriod absolute() const { return TimePeriod(true, m_iWhole100Nanoseconds, m_fFractionOf100Nanoseconds); }

	Ascii toString() const;

	TimePeriod() : m_bSign(true), m_iWhole100Nanoseconds(0), m_fFractionOf100Nanoseconds(0) {}

private:
	
	TimePeriod(bool bSign, UInt64 iWhole100Nanoseconds, Real64 fFractionOf100Nanoseconds)
		: m_bSign(bSign)
		, m_iWhole100Nanoseconds(iWhole100Nanoseconds)
		, m_fFractionOf100Nanoseconds(fFractionOf100Nanoseconds)
	{

	}

	bool m_bSign;
	UInt64 m_iWhole100Nanoseconds;
	Real64 m_fFractionOf100Nanoseconds;

	friend class TimePoint;
};

DeclComparable(TimePeriod);

inline TimePeriod operator- (const TimePeriod& a, const TimePeriod& b) { return a + (-b); }
inline TimePeriod operator* (Real64 a, const TimePeriod& b) { return b * a; }
inline TimePeriod operator/ (const TimePeriod& num, Real64 div) { return num * (Real64(1) / div); }

inline bool operator== (const TimePeriod& a, const TimePeriod& b) { return TimePeriod::comparef(a,b) == 0; } 
inline bool operator!= (const TimePeriod& a, const TimePeriod& b) { return TimePeriod::comparef(a,b) != 0; } 
inline bool operator> (const TimePeriod& a, const TimePeriod& b) { return TimePeriod::comparef(a,b) > 0; } 
inline bool operator< (const TimePeriod& a, const TimePeriod& b) { return TimePeriod::comparef(a,b) < 0; } 
inline bool operator<= (const TimePeriod& a, const TimePeriod& b) { return TimePeriod::comparef(a,b) <= 0; } 
inline bool operator>= (const TimePeriod& a, const TimePeriod& b) { return TimePeriod::comparef(a,b) >= 0; } 

inline TimePeriod TimePeriod::notime() { return TimePeriod(true, 0, 0); }
inline TimePeriod TimePeriod::foreverFuture() { return TimePeriod(true, 0xFFFFFFFFFFFFFFFFULL, 1.0); }
inline TimePeriod TimePeriod::foreverPast() { return TimePeriod(false, 0xFFFFFFFFFFFFFFFFULL, 1.0); }

inline TimePeriod
TimePeriod::years(Real64 fNumYears)
{
	return days(fNumYears * Real64(365.2425));
}

inline TimePeriod
TimePeriod::months(Real64 fNumMonths)
{
	return years(fNumMonths / Real64(12));
}

inline TimePeriod
TimePeriod::weeks(Real64 fNumWeeks)
{
	return days(fNumWeeks * Real64(7));
}

inline TimePeriod
TimePeriod::days(Real64 fNumDays)
{
	return hours(fNumDays * Real64(24));
}

inline TimePeriod
TimePeriod::hours(Real64 fNumHours)
{
	return minutes(fNumHours * Real64(60));
}

inline TimePeriod
TimePeriod::minutes(Real64 fNumMinutes)
{
	return seconds(fNumMinutes * Real64(60));
}

inline TimePeriod
TimePeriod::milliseconds(Real64 fNumSeconds)
{
	return seconds(fNumSeconds / Real64(1000));
}

inline TimePeriod
TimePeriod::microseconds(Real64 fNumMilliSeconds)
{
	return milliseconds(fNumMilliSeconds / Real64(1000));
}

inline TimePeriod
TimePeriod::nanoseconds(Real64 fNumNanoSeconds)
{
	return microseconds(fNumNanoSeconds / Real64(1000));
}

inline TimePeriod
TimePeriod::picoseconds(Real64 fNumPicoSeconds)
{
	return nanoseconds(fNumPicoSeconds / Real64(1000));
}

inline TimePeriod
TimePeriod::operator-() const
{
	return TimePeriod(!m_bSign, m_iWhole100Nanoseconds, m_fFractionOf100Nanoseconds);
}
