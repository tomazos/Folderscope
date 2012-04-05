// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/UTF16.h"

class Error
{
public:
	Error();
	void who(const UTF16& sLocation);
	void what(const UTF16& sErrorMessage);

	operator bool();
	operator UTF16();

private:
	Error(const Error&) { }
	UTF16 m_sErrorMessage;
	bool m_bErrorOccured;
};
