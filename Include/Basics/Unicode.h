// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Character.h"

class UTF8;
class UTF16;

class Unicode
{
public:
	static Character::ECodingResult UTF8_to_UTF16(const UTF8& sSource, UTF16& sDestination);
	static Character::ECodingResult UTF16_to_UTF8(const UTF16& sSource, UTF8& sDestination);
};

