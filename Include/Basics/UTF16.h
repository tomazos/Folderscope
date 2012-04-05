// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include <cstdarg>

#include "Basics/Blob.h"
#include "Basics/Ascii.h"
#include "Basics/Unicode.h"
#include "Basics/HexString.h"

class UTF16
{
public:
	typedef Chr16 CodeUnit;

	UTF16();
	UTF16(const Chr16* p);
	UTF16(const Ascii&);
	UTF16(const Blob&);

	inline UTF16(const UTF16&);

	inline UTF16& operator=(const UTF16&);

	inline Size length() const;
	inline bool empty() const { return 0 == length(); }

	inline const Chr16* ptr() const;
	inline Chr16 operator[](Size iIndex) const { return ptr()[iIndex]; }

	Blob asBlob() const { return m_blob; }
	Ascii asHex() const { return HexString::binToHex(asBlob()); }

	inline bool operator==(const UTF16&) const;
	inline bool operator!=(const UTF16&) const;
	inline bool operator< (const UTF16&) const;
	inline bool operator> (const UTF16&) const;
	inline bool operator<=(const UTF16&) const;
	inline bool operator>=(const UTF16&) const;

	static UTF16 format(const Chr16* sFormat, ...);
	static UTF16 vformat(const Chr16* sFormat, va_list arg);

	UTF16 operator+(const UTF16&) const;
	UTF16& operator+=(const UTF16& b) { return ((*this) = ((*this) + b)); }

	bool startsWith(const UTF16&) const;
	bool endsWith(const UTF16&) const;
	bool contains(const UTF16&) const;

	UTF16 prefix(Size iNumCodeUnits) const;
	UTF16 suffix(Size iNumCodeUnits) const;
	UTF16 substring(Size iStartCodeUnit, Size iNumCodeUnits) const;

	static UInt64 hashf(const UTF16& x) { return Blob::hashf(x.m_blob); }
	static SInt64 comparef(const UTF16& a, const UTF16& b) { return Blob::comparef(a.m_blob, b.m_blob); }

private:
	Blob m_blob;

	friend class Unicode;
};

DeclHashable(UTF16);
DeclComparable(UTF16);

inline SInt64 compareUTF16CaseInsensetive(const UTF16& a, const UTF16& b)
{
	return _wcsicmp(a.ptr(), b.ptr());
};

inline UTF16 operator+ (const Ascii& a, const UTF16& b) { return UTF16(a) + b; }
inline UTF16 operator+ (const UTF16& a, const Ascii& b) { return a + UTF16(b); }

inline UTF16::UTF16(const UTF16& that)
	: m_blob(that.m_blob)
{

}

inline UTF16&
UTF16::operator=(const UTF16& that)
{
	m_blob = that.m_blob;
	return (*this);
}

inline const Chr16*
UTF16::ptr() const
{
	return (wchar_t*) m_blob.ptr();
}

inline Size
UTF16::length() const
{
	return (m_blob.length() / 2) - 1;
}

inline bool
UTF16::operator==(const UTF16& that) const
{
	return m_blob == that.m_blob;
}

inline bool
UTF16::operator!=(const UTF16& that) const
{
	return m_blob != that.m_blob;
}

inline bool
UTF16::operator< (const UTF16& that) const
{
	return m_blob < that.m_blob;
}

inline bool
UTF16::operator> (const UTF16& that) const
{
	return m_blob > that.m_blob;
}

inline bool
UTF16::operator<=(const UTF16& that) const
{
	return m_blob <= that.m_blob;
}

inline bool
UTF16::operator>=(const UTF16& that) const
{
	return m_blob >= that.m_blob;
}
