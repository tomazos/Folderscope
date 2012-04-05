// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FileIndexInfo.h"

#pragma once
#include "global.h"

#include "Basics.h"

FileIndexInfo::FileIndexInfo()
	: m_bIsValid(false)
{}

FileIndexInfo::FileIndexInfo(UInt32 iVolumeSerialNumber, UInt32 iFileIndexHigh, UInt32 iFileIndexLow)
	: m_bIsValid(true)
	, m_iVolumeSerialNumber(iVolumeSerialNumber)
	, m_iFileIndexHigh(iFileIndexHigh)
	, m_iFileIndexLow(iFileIndexLow)
{}

bool FileIndexInfo::operator == (const FileIndexInfo& that) const
{
	return 
		m_iFileIndexLow == that.m_iFileIndexLow &&
		m_iFileIndexHigh == that.m_iFileIndexHigh &&
		m_iVolumeSerialNumber == that.m_iVolumeSerialNumber;
}

bool FileIndexInfo::operator != (const FileIndexInfo& that) const
{
	return !((*this) == that);
}

bool FileIndexInfo::operator < (const FileIndexInfo& that) const
{
	if (m_iFileIndexLow != that.m_iFileIndexLow)
		return (m_iFileIndexLow < that.m_iFileIndexLow);
	
	if (m_iFileIndexHigh != that.m_iFileIndexHigh)
		return (m_iFileIndexHigh < that.m_iFileIndexHigh);

	return m_iVolumeSerialNumber < that.m_iVolumeSerialNumber;
}

bool FileIndexInfo::operator > (const FileIndexInfo& that) const
{
	return that < (*this);
}

bool FileIndexInfo::operator <= (const FileIndexInfo& that) const
{
	return ! (that < (*this));
}

bool FileIndexInfo::operator >= (const FileIndexInfo& that) const
{
	return ! ((*this) < that);
}

SInt64 FileIndexInfo::comparef(const FileIndexInfo& a, const FileIndexInfo& b)
{
	SInt64 iRes = SInt64(a.m_iFileIndexLow) - SInt64(b.m_iFileIndexLow);
	
	if (iRes != 0)
		return iRes;
	
	iRes = SInt64(a.m_iFileIndexHigh) - SInt64(b.m_iFileIndexHigh);
	
	if (iRes != 0)
		return iRes;

	return SInt64(a.m_iVolumeSerialNumber) - SInt64(b.m_iVolumeSerialNumber);
}

UInt64 FileIndexInfo::hashf(const FileIndexInfo& x)
{
	UInt64 h = 0xA4238B58310421EFULL;
	UInt8* p;
	p = (UInt8*) &x.m_iVolumeSerialNumber;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	p = (UInt8*) &x.m_iFileIndexHigh;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	p = (UInt8*)  &x.m_iFileIndexLow;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	p++;
	h += ((h << 5) ^ UInt64(*p));
	return h;
}

UTF16 FileIndexInfo::getString()
{
	return UTF16::format(L"volume = %.8X file index = %.8X-%.8X", m_iVolumeSerialNumber, m_iFileIndexHigh, m_iFileIndexLow);
}
