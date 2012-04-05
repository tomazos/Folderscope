// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class Version
{
public:
	enum EType
	{
		eDevelopment,
		eBeta,
		eRelease,
		eInvalid
	};

	Version() : m_eType(eInvalid) {}
	Version(const UTF8& sVersionString);
	Version(const UTF16& sVersionString);
	Version(const Ascii& sVersionString);

	operator Ascii() const;
	operator UTF8() const { return UTF8(Ascii(*this)); }
	operator UTF16() const { return UTF16(Ascii(*this)); }

	bool operator ==(const Version&) const;
	bool operator <(const Version&) const;

	bool isValid() const { return m_eType != eInvalid; }
	bool isInvalid() const { return m_eType == eInvalid; }

private:
	void parse(const Chr8* sVersionString);
	class ParsingError {};

	const Chr8* m_p;
	Chr8 peek() { return *m_p; }
	Chr8 pop() { return *(m_p++); }

	int parseNaturalNumber();
	int parseNonnegNumber();
	int parseDigit();

	void parsePeriod();
	UInt32 m_iMajor;
	UInt32 m_iMinor;
	UInt32 m_iPoint;
	EType m_eType;
	UInt32 m_iRevision;
};

inline bool operator!=(const Version& a, const Version& b) { return !(a == b); }
inline bool operator>(const Version& a, const Version& b) { return !(a == b || a < b); }
inline bool operator<=(const Version& a, const Version& b) { return !(a > b); }
inline bool operator>=(const Version& a, const Version& b) { return !(a < b); }
