// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Error.h"

Error::Error()
	: m_bErrorOccured(false)
{}

void Error::who(const UTF16& sLocation)
{
	m_bErrorOccured = true;
	m_sErrorMessage = sLocation + L": " + m_sErrorMessage;
}

void Error::what(const UTF16& sErrorMessage)
{
	m_bErrorOccured = true;
	m_sErrorMessage = sErrorMessage;
}

Error::operator bool()
{
	return m_bErrorOccured;
}

Error::operator UTF16()
{
	return m_sErrorMessage;
}
