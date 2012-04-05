// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Ascii.h"

#include "Basics/Memory.h"

#include <cstring>
#include "stdio.h"

#include "Basics/StringOps.h"

Ascii::Ascii()
	: m_blob()
{
	Chr8* p = (Chr8*) NewBytes(1);
	p[0] = 0;
	m_blob = Blob::own(p,1);
}

Ascii::Ascii(const Chr8* s)
	: m_blob()
{
	Size iLength = std::strlen(s);
	Chr8* p = (Chr8*) NewBytes(iLength+1);
	Memory::copy(s,p,iLength+1);
	m_blob = Blob::own(p,iLength+1);
}

Ascii
Ascii::format(const Chr8* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);
	Ascii sRet = vformat(sFormat, args);
	va_end(args);
	return sRet;
}

Ascii
Ascii::vformat(const Chr8* sFormat, va_list args)
{
	Size iLength = _vscprintf(sFormat, args);
	Chr8* pBuf = (Chr8*) NewBytes(iLength+1);
	vsprintf_s(pBuf, iLength + 1, sFormat, args);

	Ascii s;
	s.m_blob = Blob::own(pBuf,iLength+1);
	return s;
}

Ascii
Ascii::operator+(const Ascii& b) const
{
	return StringOps<Ascii>::concat(*this, b);
}

bool
Ascii::startsWith(const Ascii& sPrefix) const
{
	return StringOps<Ascii>::startsWith(*this, sPrefix);
}

bool
Ascii::endsWith(const Ascii& sSuffix) const
{
	return StringOps<Ascii>::endsWith(*this, sSuffix);
}

bool
Ascii::contains(const Ascii& sSubstring) const
{
	return StringOps<Ascii>::contains(*this, sSubstring);
}

Ascii
Ascii::prefix(Size iNumCodeUnits) const
{
	return StringOps<Ascii>::prefix(*this, iNumCodeUnits);
}

Ascii
Ascii::suffix(Size iNumCodeUnits) const
{
	return StringOps<Ascii>::suffix(*this, iNumCodeUnits);
}

Ascii
Ascii::substring(Size iStartCodeUnit, Size iNumCodeUnits) const
{
	return StringOps<Ascii>::substring(*this, iStartCodeUnit, iNumCodeUnits);
}
