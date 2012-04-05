// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/TimePeriod.h"

#include <cmath>

TimePeriod
TimePeriod::seconds(Real64 fNumSeconds)
{
	if (fNumSeconds == Real64(0))
	{
		return TimePeriod(true, 0, 0);
	}
	else
	{
		bool bSign = (fNumSeconds > 0);

		Real64 fAll = std::fabs(fNumSeconds * Real64(10000000));
		Real64 fWhole = std::floor(fAll);
		Real64 fFrac = fAll - fWhole;

		return TimePeriod(bSign, UInt64(fWhole), fFrac);
	}
}

TimePeriod
TimePeriod::operator*(Real64 fInFactor) const
{
	if (fInFactor == Real64(0))
		return TimePeriod(true,0,0);

	if ((*this) == foreverFuture() && fInFactor > 0)
		return foreverFuture();

	if ((*this) == foreverFuture() && fInFactor < 0)
		return foreverPast();

	if ((*this) == foreverPast() && fInFactor > 0)
		return foreverPast();

	if ((*this) == foreverPast() && fInFactor < 0)
		return foreverFuture();

	bool bSign = m_bSign;

	if (fInFactor < Real64(0))
		bSign = !bSign;

	Real64 fAbsFactor = std::fabs(fInFactor);

	Real64 fA = Real64(m_iWhole100Nanoseconds & 0xFFFFFFFF00000000ULL);
	Real64 fB = Real64(m_iWhole100Nanoseconds & 0x00000000FFFFFFFFULL);
	Real64 fC = m_fFractionOf100Nanoseconds;

	fA *= fAbsFactor;
	fB *= fAbsFactor;
	fC *= fAbsFactor;

	UInt64 iWhole = UInt64(std::floor(fA)) + UInt64(std::floor(fB)) + UInt64(std::floor(fC));

	fA -= std::floor(fA);
	fB -= std::floor(fB);
	fC -= std::floor(fC);

	Real64 fFrac = fA + fB + fC;

	return TimePeriod(bSign, iWhole, fFrac);
}

Ascii TimePeriod::toString() const
{
	TimePeriod t = absolute();

	if (t == TimePeriod::notime())
	{
		return "notime";
	}
	else if (t > TimePeriod::years(2))
	{
		UInt64 iYears = UInt64(std::floor(t / TimePeriod::years(1)));
		UInt64 iMonths = UInt64((t - TimePeriod::years(Real64(iYears))) / TimePeriod::months(1));
		return Ascii::format("%s%lluyrs %llumths", m_bSign ? "" : "- ", iYears, iMonths);
	}
	else if (t > TimePeriod::months(2))
	{
		UInt64 iMonths = UInt64(std::floor(t / TimePeriod::months(1)));
		UInt64 iDays = UInt64((t - TimePeriod::months(Real64(iMonths))) / TimePeriod::days(1));
		return Ascii::format("%s%llumths %lludays", m_bSign ? "" : "- ", iMonths, iDays);
	}
	else if (t > TimePeriod::weeks(2))
	{
		UInt64 iWeeks = UInt64(std::floor(t / TimePeriod::weeks(1)));
		UInt64 iDays = UInt64((t - TimePeriod::weeks(Real64(iWeeks))) / TimePeriod::days(1));
		return Ascii::format("%s%lluwks %lludays", m_bSign ? "" : "- ", iWeeks, iDays);
	}
	else if (t >= TimePeriod::days(1))
	{
		UInt64 iDays = UInt64(std::floor(t / TimePeriod::days(1)));
		UInt64 iHours = UInt64((t - TimePeriod::days(Real64(iDays))) / TimePeriod::hours(1));
		return Ascii::format("%s%lludays %lluh", m_bSign ? "" : "- ", iDays, iHours);
	}
	else if (t >= TimePeriod::hours(1))
	{
		UInt64 iHours = UInt64(std::floor(t / TimePeriod::hours(1)));
		UInt64 iMinutes = UInt64((t - TimePeriod::hours(Real64(iHours))) / TimePeriod::minutes(1));
		return Ascii::format("%s%lluh %llum", m_bSign ? "" : "- ", iHours, iMinutes);
	}
	else if (t >= TimePeriod::minutes(1))
	{
		UInt64 iMinutes = UInt64(std::floor(t / TimePeriod::minutes(1)));
		UInt64 iSeconds = UInt64((t - TimePeriod::minutes(Real64(iMinutes))) / TimePeriod::seconds(1));
		return Ascii::format("%s%llum %llus", m_bSign ? "" : "- ", iMinutes, iSeconds);
	}
	else if (t >= TimePeriod::seconds(1))
	{
		return Ascii::format("%s%.3fs", m_bSign ? "" : "-", t / TimePeriod::seconds(1));
	}
	else if (t >= TimePeriod::milliseconds(1))
	{
		return Ascii::format("%s%.3fms", m_bSign ? "" : "-", t / TimePeriod::milliseconds(1));
	}
	else if (t >= TimePeriod::microseconds(1))
	{
		return Ascii::format("%s%.3fus", m_bSign ? "" : "-", t / TimePeriod::microseconds(1));
	}
	else if (t >= TimePeriod::nanoseconds(1))
	{
		return Ascii::format("%s%.3fns", m_bSign ? "" : "-", t / TimePeriod::nanoseconds(1));
	}
	else
	{
		return Ascii::format("%s%gps", m_bSign ? "" : "-", t / TimePeriod::picoseconds(1));
	}
}

TimePeriod
TimePeriod::operator+(const TimePeriod& b) const
{
	const TimePeriod& a = (*this);

	if (a == foreverFuture() && b == foreverPast())
		return notime();

	if (b == foreverFuture() && a == foreverPast())
		return notime();

	if (a == foreverFuture() || b == foreverFuture())
		return foreverFuture();

	if (a == foreverPast() || b == foreverPast())
		return foreverPast();

	bool bSign;
	UInt64 iWhole;
	Real64 fFrac;

	if (a.m_bSign == b.m_bSign)
	{
		bSign = a.m_bSign; // = b.m_bSign

		iWhole = a.m_iWhole100Nanoseconds + b.m_iWhole100Nanoseconds;
		fFrac = a.m_fFractionOf100Nanoseconds + b.m_fFractionOf100Nanoseconds;

		if (fFrac >= Real64(1))
		{
			fFrac -= Real64(1);
			iWhole++;
		}
	}
	else
	{
		if (a.m_iWhole100Nanoseconds == b.m_iWhole100Nanoseconds)
		{
			if (a.m_fFractionOf100Nanoseconds == b.m_fFractionOf100Nanoseconds)
			{
				bSign = true;
				iWhole = 0;
				fFrac = 0;
			}
			else if (a.m_fFractionOf100Nanoseconds > b.m_fFractionOf100Nanoseconds)
			{
				bSign = a.m_bSign;
				iWhole = 0;
				fFrac = a.m_fFractionOf100Nanoseconds - b.m_fFractionOf100Nanoseconds;
			}
			else // a.m_fFractionOf100Nanoseconds < b.m_fFractionOf100Nanoseconds
			{
				bSign = b.m_bSign;
				iWhole = 0;
				fFrac = b.m_fFractionOf100Nanoseconds - a.m_fFractionOf100Nanoseconds;
			}
		}
		else if (a.m_iWhole100Nanoseconds > b.m_iWhole100Nanoseconds)
		{
			bSign = a.m_bSign;
			iWhole = a.m_iWhole100Nanoseconds - b.m_iWhole100Nanoseconds;
			fFrac = a.m_fFractionOf100Nanoseconds - b.m_fFractionOf100Nanoseconds;
			if (fFrac < Real64(0))
			{
				fFrac += Real64(1);
				iWhole--;
			}
		}
		else // (a.m_iWhole100Nanoseconds < b.m_iWhole100Nanoseconds)
		{
			bSign = b.m_bSign;
			iWhole = b.m_iWhole100Nanoseconds - a.m_iWhole100Nanoseconds;
			fFrac = b.m_fFractionOf100Nanoseconds - a.m_fFractionOf100Nanoseconds;
			if (fFrac < Real64(0))
			{
				fFrac += Real64(1);
				iWhole--;
			}
		}
	}
	return TimePeriod(bSign, iWhole, fFrac);
}

SInt64 TimePeriod::comparef(const TimePeriod& a, const TimePeriod& b)
{
	if (a.m_bSign != b.m_bSign)
		return (a.m_bSign ? 1 : -1);

	SInt64 retVal = (a.m_bSign ? 1 : -1);

	if (a.m_iWhole100Nanoseconds > b.m_iWhole100Nanoseconds)
		return retVal;
	else if (a.m_iWhole100Nanoseconds < b.m_iWhole100Nanoseconds)
		return -retVal;
	else
	{
		if (a.m_fFractionOf100Nanoseconds > b.m_fFractionOf100Nanoseconds)
			return retVal;
		else if (a.m_fFractionOf100Nanoseconds < b.m_fFractionOf100Nanoseconds)
			return -retVal;
		else
			return 0;
	}
}

Real64
TimePeriod::operator/(const TimePeriod& divisor) const
{
	if (divisor == foreverFuture() || divisor == foreverPast())
		return 0;

	Real64 fDivisor = Real64(divisor.m_iWhole100Nanoseconds) + divisor.m_fFractionOf100Nanoseconds;

	return Real64(m_iWhole100Nanoseconds & 0xFFFFFFFF00000000ULL) / fDivisor
		 + Real64(m_iWhole100Nanoseconds & 0x00000000FFFFFFFFULL) / fDivisor
		 + m_fFractionOf100Nanoseconds / fDivisor;
}
