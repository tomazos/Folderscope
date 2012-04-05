// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/TimePoint.h"

#include <cmath>

#include "Basics/Buffer.h"

#include "Basics/TickUnit.h"

TimePoint
TimePoint::now()
{
    FILETIME ft;

	GetSystemTimeAsFileTime(&ft);

	UInt64 iWhole = (UInt64(ft.dwHighDateTime) << 32) | UInt64(ft.dwLowDateTime);

	UInt64 iTicks = X86_RDTSC();

	return TimePoint(TimePeriod(true, iWhole, 0), iTicks);
}

TimePoint TimePoint::foreverFuture()
{
	return TimePoint(TimePeriod::foreverFuture(), UInt64(1) << 62);
}

TimePoint TimePoint::foreverPast()
{
	return TimePoint(TimePeriod::foreverPast(), UInt64(1) << 62);
}

TimePoint
TimePoint::make(UInt16 iYear, UInt8 iMonth, UInt8 iDay, UInt8 iHour, UInt8 iMinute, Real64 fSeconds)
{
    FILETIME ft;
	SYSTEMTIME st;

	st.wYear = iYear;
	st.wMonth = iMonth;
	st.wDay = iDay;
	st.wHour = iHour;
	st.wMinute = iMinute;
	st.wDayOfWeek = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;
    SystemTimeToFileTime(&st, &ft);

	UInt64 iWhole = (UInt64(ft.dwHighDateTime) << 32) | UInt64(ft.dwLowDateTime);

	iWhole += UInt64(std::floor(fSeconds / 0.0000001));

	Real64 fFrac = (fSeconds / 0.0000001) - std::floor(fSeconds / 0.0000001);

	return TimePoint(TimePeriod(true, iWhole, fFrac), UInt64(1) << 62);
};

Ascii TimePoint::toString(const Chr8* fmt) const
{
	FILETIME utcft, localft;
	SYSTEMTIME st;
	utcft.dwHighDateTime = UInt32(m_period.m_iWhole100Nanoseconds >> 32);
	utcft.dwLowDateTime = UInt32(m_period.m_iWhole100Nanoseconds);
	FileTimeToLocalFileTime(&utcft,&localft);
	FileTimeToSystemTime(&localft,&st);

	Ascii format(fmt);

	Buffer buf(format.length() * 4 + 1);
	Chr8* p = (Chr8*) buf.ptr();

	for (int i = 0; i < format.length(); i++)
	{
		Ascii sNextPiece;
		Chr8 c = format[i];
		switch (c)
		{
		case 'Y' : sNextPiece = Ascii::format("%04hd", st.wYear); break;
		case 'M' : sNextPiece = Ascii::format("%02hd", st.wMonth); break;
		case 'D' : sNextPiece = Ascii::format("%02hd", st.wDay); break;
		case 'h' : sNextPiece = Ascii::format("%02hd", st.wHour); break;
		case 'm' : sNextPiece = Ascii::format("%02hd", st.wMinute); break;
		case 's' : sNextPiece = Ascii::format("%02hd", st.wSecond); break;
		case 't' : sNextPiece = Ascii::format("%03hd", st.wMilliseconds); break;
		default: (*p++) = c;
		}
		Memory::copy(sNextPiece.ptr(), p, sNextPiece.length());
		p += sNextPiece.length();
	}

	(*p) = '\0';

	buf.trimString<Chr8>();

	return Ascii(Blob(buf));
}

TimePoint
TimePoint::operator+(const TimePeriod& period) const
{
	return TimePoint(m_period + period, m_iTicks);
}

TimePeriod
TimePoint::operator-(const TimePoint& b) const
{
	TimePeriod bigPeriod = m_period - b.m_period;

	SInt64 iTickDiff = m_iTicks - b.m_iTicks;

	TimePeriod littlePeriod = Real64(iTickDiff) * (TickUnit::instance()->get());

	TimePeriod periodDiff = (bigPeriod - littlePeriod).absolute();

	if (periodDiff < TimePeriod::seconds(1) && littlePeriod != TimePeriod::notime())
		return littlePeriod;
	else
		return bigPeriod;
}
