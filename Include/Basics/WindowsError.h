// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/UTF16.h"

class WindowsError
{
public:
	static UTF16 getLastError();
	static UTF16 codeToString(int iErrorCode);
};
