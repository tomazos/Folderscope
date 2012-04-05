// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/UTF8.h"

#include <cstring>

#include "Basics/Memory.h"

#include "stdio.h"

#include "Basics/StringOps.h"

UTF8::operator UTF16() const
{
	UTF16 sUTF16;
	if (Unicode::UTF8_to_UTF16(*this, sUTF16) == Character::eCodingFailure)
		return UTF16(HexString::binToHex(asBlob()));
	else
		return sUTF16;
}

UTF8::UTF8(const UTF16& sUTF8)
{
	if (Unicode::UTF16_to_UTF8(sUTF8, *this) == Character::eCodingFailure)
		(*this) = UTF8(HexString::binToHex(sUTF8.asBlob()));
}

UTF8::UTF8()
	: m_blob()
{
	Chr8* p = (Chr8*) NewBytes(1);
	p[0] = 0;
	m_blob = Blob::own(p,1);
}

UTF8::UTF8(const Chr8* s)
	: m_blob()
{
	Size iLength = std::strlen(s);
	Chr8* p = (Chr8*) NewBytes(iLength+1);
	Memory::copy(s,p,iLength+1);
	m_blob = Blob::own(p,iLength+1);
}

UTF8
UTF8::format(const Chr8* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);
	UTF8 sRet = vformat(sFormat, args);
	va_end(args);
	return sRet;
}

UTF8
UTF8::vformat(const Chr8* sFormat, va_list args)
{
	Size iLength = _vscprintf(sFormat, args);
	Chr8* pBuf = (Chr8*) NewBytes(iLength+1);
	vsprintf_s(pBuf, iLength + 1, sFormat, args);

	UTF8 s;
	s.m_blob = Blob::own(pBuf,iLength+1);
	return s;
}

UTF8
UTF8::operator+(const UTF8& b) const
{
	return StringOps<UTF8>::concat(*this, b);
}

bool
UTF8::startsWith(const UTF8& sPrefix) const
{
	return StringOps<UTF8>::startsWith(*this, sPrefix);
}

bool
UTF8::endsWith(const UTF8& sSuffix) const
{
	return StringOps<UTF8>::endsWith(*this, sSuffix);
}

bool
UTF8::contains(const UTF8& sSubstring) const
{
	return StringOps<UTF8>::contains(*this, sSubstring);
}

UTF8
UTF8::prefix(Size iNumCodeUnits) const
{
	return StringOps<UTF8>::prefix(*this, iNumCodeUnits);
}

UTF8
UTF8::suffix(Size iNumCodeUnits) const
{
	return StringOps<UTF8>::suffix(*this, iNumCodeUnits);
}

UTF8
UTF8::substring(Size iStartCodeUnit, Size iNumCodeUnits) const
{
	return StringOps<UTF8>::substring(*this, iStartCodeUnit, iNumCodeUnits);
}
