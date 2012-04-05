// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/Version.h"

Version::Version(const UTF8& sVersionString)
	: m_eType(eInvalid)
{
	parse(sVersionString.ptr());
}

Version::Version(const UTF16& sVersionString)
	: m_eType(eInvalid)
{
	
	UTF8 sVersionStringUTF8;
	if (Unicode::UTF16_to_UTF8(sVersionString, sVersionStringUTF8) == Character::eCodingSuccess)
		parse(sVersionStringUTF8.ptr());
}

Version::Version(const Ascii& sVersionString)
	: m_eType(eInvalid)
{
	parse(sVersionString.ptr());
}

Version::operator Ascii() const
{
	if (m_iPoint == 0)
	{
		switch (m_eType)
		{
			case eDevelopment : return Ascii::format("%d.%dd%d", UInt32(m_iMajor), UInt32(m_iMinor), UInt32(m_iRevision));
			case eBeta : return Ascii::format("%d.%db%d", UInt32(m_iMajor), UInt32(m_iMinor), UInt32(m_iRevision));
			case eRelease : return Ascii::format("%d.%d", UInt32(m_iMajor), UInt32(m_iMinor));
			default : return "invalid";
		}
	}
	else
	{
		switch (m_eType)
		{
			case eDevelopment : return Ascii::format("%d.%d.%dd%d", UInt32(m_iMajor), UInt32(m_iMinor), UInt32(m_iPoint), UInt32(m_iRevision));
			case eBeta : return Ascii::format("%d.%d.%db%d", UInt32(m_iMajor), UInt32(m_iMinor), UInt32(m_iPoint), UInt32(m_iRevision));
			case eRelease : return Ascii::format("%d.%d.%d", UInt32(m_iMajor), UInt32(m_iMinor), UInt32(m_iPoint));
			default : return "invalid";
		}
	}

}

bool Version::operator ==(const Version& b) const
{
	const Version& a = (*this);

	if (a.isInvalid() && b.isInvalid())
		return true;

	if (a.isValid() && b.isInvalid())
		return false;

	if (a.isInvalid() && b.isValid())
		return false;

	if (a.m_iMajor != b.m_iMajor)
		return false;

	if (a.m_iMinor != b.m_iMinor)
		return false;

	if (a.m_iPoint != b.m_iPoint)
		return false;

	if (a.m_eType != b.m_eType)
		return false;

	if (m_eType == eRelease)
		return true;

	if (a.m_iRevision != b.m_iRevision)
		return false;

	return true;
}

bool Version::operator <(const Version& b) const
{
	const Version& a = (*this);

	if (a.isInvalid() && b.isInvalid())
		return false;

	if (a.isValid() && b.isInvalid())
		return false;

	if (a.isInvalid() && b.isValid())
		return true;

	if (a.m_iMajor < b.m_iMajor)
		return true;
	else if (a.m_iMajor > b.m_iMajor)
		return false;

	if (a.m_iMinor < b.m_iMinor)
		return true;
	else if (a.m_iMinor > b.m_iMinor)
		return false;

	if (a.m_iPoint < b.m_iPoint)
		return true;
	else if (a.m_iPoint > b.m_iPoint)
		return false;

	if (a.m_eType == eRelease && b.m_eType == eRelease)
		return false;

	if (a.m_eType == eRelease)
		return false;

	if (b.m_eType == eRelease)
		return true;

	if (a.m_eType == eBeta && b.m_eType == eBeta)
		return a.m_iRevision < b.m_iRevision;

	if (a.m_eType == eBeta)
		return false;

	if (b.m_eType == eBeta)
		return true;

	return a.m_iRevision < b.m_iRevision;
}

void Version::parse(const Chr8* sVersionString)
{
	try
	{
		m_p = sVersionString;

		m_iMajor = parseNaturalNumber();
		parsePeriod();
		m_iMinor = parseNonnegNumber();
		
		m_iPoint = 0;
		m_iRevision = 0;
		m_eType = eInvalid;

		if (peek() == '.')
		{
			parsePeriod();
			m_iPoint = parseNaturalNumber();
		}
		switch(pop())
		{
		case '\0':
			m_eType = eRelease;
			m_iRevision = 0;
			return;
		case 'b':
			m_eType = eBeta;
			m_iRevision = parseNaturalNumber();
			break;
		case 'd':
			m_eType = eDevelopment;
			m_iRevision = parseNaturalNumber();
			break;
		default : throw ParsingError();
		}

		if (peek() != '\0')
			throw ParsingError();
	}
	catch (ParsingError&)
	{
		m_eType = eInvalid;
	}
}

int Version::parseNaturalNumber()
{
	const Chr8* p = m_p;

	int iResult = parseNonnegNumber();

	if (iResult == 0)
	{
		m_p = p;
		throw ParsingError();
	}

	return iResult;
}

int Version::parseNonnegNumber()
{
	const Chr8* p = m_p;

	int iResult = parseDigit();

	if (iResult == 0)
	{
		try
		{
			parseDigit();
		}
		catch (ParsingError&)
		{
			return 0;
		}
		m_p = p;
		throw ParsingError();
	}

	while (iResult < 1000)
	{
		try
		{
			int iNextDigit = parseDigit();
			iResult *= 10;
			iResult += iNextDigit;
		}
		catch (ParsingError&)
		{
			return iResult;
		}
	}

	m_p = p;
	throw ParsingError();
}

int Version::parseDigit()
{
	const Chr8* p = m_p;

	UInt8 i;
	if (Character::decDigit(pop(), i) == Character::eCodingSuccess)
		return i;
	else
	{
		m_p = p;
		throw ParsingError();
	}
}

void Version::parsePeriod()
{
	if (peek() != '.')
		throw ParsingError();

	pop();
}
