// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Blob.h"
#include "Basics/Ascii.h"
#include "Basics/Character.h"

class HexString
{
public:
	static Character::ECodingResult hexToBin(const Ascii& sHex, Blob& out);
	static Blob hexToBin(const Ascii& sHex)
	{
		Blob b;
		hexToBin(sHex,b);
		return b;
	}

	static Ascii binToHex(const Blob&);
};
