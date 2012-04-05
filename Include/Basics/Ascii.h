// (C) 2009 Tomazos-Jager Software Labs
#pragma once
#include "global.h"

#include <cstdarg>

#include "Basics/Blob.h"
#include "Basics/DefaultHasher.h"
#include "Basics/DefaultComparator.h"

class Ascii
{
public:
	typedef Chr8 CodeUnit;

	Ascii();
	Ascii(const Chr8*);
	inline Ascii(const Ascii&);
	inline explicit Ascii(const Blob&);
	inline Ascii& operator=(const Ascii&);

	inline Size length() const;
	inline bool empty() const { return 0 == length(); }

	inline const Chr8* ptr() const;
	Blob asBlob() const { return m_blob; }

	inline Chr8 operator[](Size) const;

	static Ascii format(const Chr8* sFormat, ...);
	static Ascii vformat(const Chr8* sFormat, va_list arg);

	inline bool operator==(const Ascii&) const;
	inline bool operator!=(const Ascii&) const;
	inline bool operator< (const Ascii&) const;
	inline bool operator> (const Ascii&) const;
	inline bool operator<=(const Ascii&) const;
	inline bool operator>=(const Ascii&) const;

	Ascii operator+(const Ascii&) const;
	Ascii& operator+=(const Ascii& b) { return ((*this) = ((*this) + b)); }

	bool startsWith(const Ascii&) const;
	bool endsWith(const Ascii&) const;
	bool contains(const Ascii&) const;

	Ascii prefix(Size iNumCodeUnits) const;
	Ascii suffix(Size iNumCodeUnits) const;
	Ascii substring(Size iStartCodeUnit, Size iNumCodeUnits) const;

	static UInt64 hashf(const Ascii& x) { return Blob::hashf(x.m_blob); }
	static SInt64 comparef(const Ascii& a, const Ascii& b) { return Blob::comparef(a.m_blob, b.m_blob); }

private:
	Blob m_blob;
};

DeclHashable(Ascii);
DeclComparable(Ascii);

inline Ascii::Ascii(const Ascii& that)
	: m_blob(that.m_blob)
{

}

inline Ascii&
Ascii::operator=(const Ascii& that)
{
	m_blob = that.m_blob;
	return (*this);
}

inline Ascii::Ascii(const Blob& blob)
	: m_blob(blob)
{

}

inline const Chr8*
Ascii::ptr() const
{
	return (Chr8*) m_blob.ptr();
}

inline Size
Ascii::length() const
{
	return m_blob.length() - 1;
}

inline Chr8
Ascii::operator[](Size i) const
{
	return ((const Chr8*)m_blob.ptr())[i];
}

inline bool
Ascii::operator==(const Ascii& that) const
{
	return m_blob == that.m_blob;
}

inline bool
Ascii::operator!=(const Ascii& that) const
{
	return m_blob != that.m_blob;
}

inline bool
Ascii::operator< (const Ascii& that) const
{
	return m_blob < that.m_blob;
}

inline bool
Ascii::operator> (const Ascii& that) const
{
	return m_blob > that.m_blob;
}

inline bool
Ascii::operator<=(const Ascii& that) const
{
	return m_blob <= that.m_blob;
}

inline bool
Ascii::operator>=(const Ascii& that) const
{
	return m_blob >= that.m_blob;
}
