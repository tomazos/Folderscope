// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/UTF16.h"

#include "Basics/Memory.h"
#include "Basics/Buffer.h"

#include <cstring>
#include "stdio.h"

#include "Basics/Check.h"
#include "Basics/StringOps.h"

UTF16::UTF16()
	: m_blob()
{
	Chr16* p = (Chr16*) NewBytes(2);
	p[0] = 0;
	m_blob = Blob::own(p,2);
	Check(length() >= 0);
}

UTF16::UTF16(const Chr16* s)
	: m_blob()
{
	int iByteLength = wcslen(s) * 2 + 2;
	void* p = NewBytes(iByteLength);
	Memory::copy(s,p,iByteLength);
	m_blob = Blob::own(p,iByteLength);
	Check(length() >= 0);
}

UTF16::UTF16(const Ascii& s)
	: m_blob()
{
	int iByteLen = s.length()*2 + 2;

	Chr16* b = (Chr16*) NewBytes(iByteLen);
	b[s.length()] = 0;

	for (Size i = 0; i < s.length(); i++)
		b[i] = s[i];

	m_blob = Blob::own(b,iByteLen);

	Check(length() >= 0);
}


inline UTF16::UTF16(const Blob& blob)
    : m_blob(blob)
{
	Check (length() >= 0);
}

UTF16
UTF16::format(const Chr16* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);
	UTF16 sRet = vformat(sFormat, args);
	va_end(args);
	return sRet;
}

UTF16
UTF16::vformat(const Chr16* sFormat, va_list args)
{
	Size iLength = _vscwprintf(sFormat, args);
	Chr16* pBuf = (Chr16*) NewBytes(2*(iLength+1));
	vswprintf_s(pBuf, iLength + 1, sFormat, args);

	UTF16 s;
	s.m_blob = Blob::own(pBuf,2*(iLength+1));
	Check (s.length() >= 0);
	return s;
}

UTF16
UTF16::operator+(const UTF16& b) const
{
	return StringOps<UTF16>::concat(*this, b);
}

bool
UTF16::startsWith(const UTF16& sPrefix) const
{
	return StringOps<UTF16>::startsWith(*this, sPrefix);
}

bool
UTF16::endsWith(const UTF16& sSuffix) const
{
	return StringOps<UTF16>::endsWith(*this, sSuffix);
}

bool
UTF16::contains(const UTF16& sSubstring) const
{
	return StringOps<UTF16>::contains(*this, sSubstring);
}

UTF16
UTF16::prefix(Size iNumCodeUnits) const
{
	return StringOps<UTF16>::prefix(*this, iNumCodeUnits);
}

UTF16
UTF16::suffix(Size iNumCodeUnits) const
{
	return StringOps<UTF16>::suffix(*this, iNumCodeUnits);
}

UTF16
UTF16::substring(Size iStartCodeUnit, Size iNumCodeUnits) const
{
	return StringOps<UTF16>::substring(*this, iStartCodeUnit, iNumCodeUnits);
}
