// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class FileSize
{
public:
	FileSize(UInt64 iSize)
		: m_iSize(iSize)
	{}

	operator UInt64()
	{
		return m_iSize;
	}

	Ascii getShortString();
	Ascii getLongString();

private:
	UInt64 m_iSize;
};