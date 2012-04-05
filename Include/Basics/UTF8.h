// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Blob.h"
#include "Basics/Ascii.h"
#include "Basics/Unicode.h"
#include "Basics/HexString.h"
#include "Basics/UTF16.h"
#include "Basics/HexString.h"

class UTF8
{
public:
	typedef Chr8 CodeUnit;

	UTF8();
	explicit UTF8(const Chr8*);
	inline explicit UTF8(const Blob&);
	inline UTF8(const Ascii&);

	inline UTF8(const UTF8&);
	inline UTF8& operator=(const UTF8&);

	operator UTF16() const;
	UTF8(const UTF16&);

	inline Size length() const;
	inline bool empty() const { return 0 == length(); }

	inline const Chr8* ptr() const;
	Blob asBlob() const { return m_blob; }
	Ascii asHex() const { return HexString::binToHex(asBlob()); }

	inline Chr8 operator[](Size) const;

	static UTF8 format(const Chr8* sFormat, ...);
	static UTF8 vformat(const Chr8* sFormat, va_list arg);

	inline bool operator==(const UTF8&) const;
	inline bool operator!=(const UTF8&) const;
	inline bool operator< (const UTF8&) const;
	inline bool operator> (const UTF8&) const;
	inline bool operator<=(const UTF8&) const;
	inline bool operator>=(const UTF8&) const;

	UTF8 operator+(const UTF8&) const;
	UTF8& operator+=(const UTF8& b) { return ((*this) = ((*this) + b)); }

	bool startsWith(const UTF8&) const;
	bool endsWith(const UTF8&) const;
	bool contains(const UTF8&) const;

	UTF8 prefix(Size iNumCodeUnits) const;
	UTF8 suffix(Size iNumCodeUnits) const;
	UTF8 substring(Size iStartCodeUnit, Size iNumCodeUnits) const;

	static UInt64 hashf(const UTF8& x) { return Blob::hashf(x.m_blob); }
	static SInt64 comparef(const UTF8& a, const UTF8& b) { return Blob::comparef(a.m_blob, b.m_blob); }

private:
	Blob m_blob;

	friend class Unicode;
};
	
DeclHashable(UTF8);
DeclComparable(UTF8);

inline UTF8::UTF8(const UTF8& that)
	: m_blob(that.m_blob)
{

}

UTF8::UTF8(const Ascii& a)
	: m_blob(a.asBlob())
{

}

inline UTF8&
UTF8::operator=(const UTF8& that)
{
	m_blob = that.m_blob;
	return (*this);
}

inline UTF8::UTF8(const Blob& blob)
	: m_blob(blob)
{

}

inline const Chr8*
UTF8::ptr() const
{
	return (Chr8*) m_blob.ptr();
}

inline Size
UTF8::length() const
{
	return m_blob.length() - 1;
}

inline char
UTF8::operator[](Size i) const
{
	return ((const Chr8*)m_blob.ptr())[i];
}

inline bool
UTF8::operator==(const UTF8& that) const
{
	return m_blob == that.m_blob;
}

inline bool
UTF8::operator!=(const UTF8& that) const
{
	return m_blob != that.m_blob;
}

inline bool 
UTF8::operator< (const UTF8& that) const
{
	return m_blob < that.m_blob;
}

inline bool 
UTF8::operator> (const UTF8& that) const
{
	return m_blob > that.m_blob;
}

inline bool 
UTF8::operator<=(const UTF8& that) const
{
	return m_blob <= that.m_blob;
}

inline bool 
UTF8::operator>=(const UTF8& that) const
{
	return m_blob >= that.m_blob;
}

inline UTF8 operator+(const UTF8& a, const Ascii& b)
{
	return a + UTF8(b);
}

inline UTF8 operator+(const Ascii a, const UTF8& b)
{
	return UTF8(a) + b;
}
