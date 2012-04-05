// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

class Character
{
public:
	enum ECodingResult
	{
		eCodingSuccess,
		eCodingFailure
	};

	static bool isAlpha(Chr8 c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	static bool isAlpha(Chr16 c)
	{
		return (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z');
	}

	static bool isWhitespace(Chr8 c)
	{
		switch (c)
		{
		case 0x09: return true;
		case 0x0A: return true;
		case 0x0B: return true;
		case 0x0C: return true;
		case 0x0D: return true;
		case 0x20: return true;
		default: return false;
		};
	}

	static bool isWhitespace(Chr16 c)
	{
		switch (c)
		{
		case 0x09: return true;
		case 0x0A: return true;
		case 0x0B: return true;
		case 0x0C: return true;
		case 0x0D: return true;
		case 0x20: return true;
		default: return false;
		};
	}

	static ECodingResult decDigit(Chr8 c, UInt8& i)
	{
		switch (c)
		{
		case '0' : i=0; return eCodingSuccess;
		case '1' : i=1; return eCodingSuccess;
		case '2' : i=2; return eCodingSuccess;
		case '3' : i=3; return eCodingSuccess;
		case '4' : i=4; return eCodingSuccess;
		case '5' : i=5; return eCodingSuccess;
		case '6' : i=6; return eCodingSuccess;
		case '7' : i=7; return eCodingSuccess;
		case '8' : i=8; return eCodingSuccess;
		case '9' : i=9; return eCodingSuccess;
		default : return eCodingFailure;
		}
	}

	static ECodingResult decDigit(Chr16 c, UInt8& i)
	{
		switch (c)
		{
		case L'0' : i=0; return eCodingSuccess;
		case L'1' : i=1; return eCodingSuccess;
		case L'2' : i=2; return eCodingSuccess;
		case L'3' : i=3; return eCodingSuccess;
		case L'4' : i=4; return eCodingSuccess;
		case L'5' : i=5; return eCodingSuccess;
		case L'6' : i=6; return eCodingSuccess;
		case L'7' : i=7; return eCodingSuccess;
		case L'8' : i=8; return eCodingSuccess;
		case L'9' : i=9; return eCodingSuccess;
		default : return eCodingFailure;
		}
	}

	static ECodingResult hexDigit(Chr8 c, UInt8& i)
	{
		switch (c)
		{
		case '0' : i=0; return eCodingSuccess;
		case '1' : i=1; return eCodingSuccess;
		case '2' : i=2; return eCodingSuccess;
		case '3' : i=3; return eCodingSuccess;
		case '4' : i=4; return eCodingSuccess;
		case '5' : i=5; return eCodingSuccess;
		case '6' : i=6; return eCodingSuccess;
		case '7' : i=7; return eCodingSuccess;
		case '8' : i=8; return eCodingSuccess;
		case '9' : i=9; return eCodingSuccess;
		case 'a' : i=10; return eCodingSuccess;
		case 'b' : i=11; return eCodingSuccess;
		case 'c' : i=12; return eCodingSuccess;
		case 'd' : i=13; return eCodingSuccess;
		case 'e' : i=14; return eCodingSuccess;
		case 'f' : i=15; return eCodingSuccess;
		case 'A' : i=10; return eCodingSuccess;
		case 'B' : i=11; return eCodingSuccess;
		case 'C' : i=12; return eCodingSuccess;
		case 'D' : i=13; return eCodingSuccess;
		case 'E' : i=14; return eCodingSuccess;
		case 'F' : i=15; return eCodingSuccess;
		default : return eCodingFailure;
		}
	}

	static ECodingResult hexDigit(Chr16 c, UInt8& i)
	{
		switch (c)
		{
		case L'0' : i=0; return eCodingSuccess;
		case L'1' : i=1; return eCodingSuccess;
		case L'2' : i=2; return eCodingSuccess;
		case L'3' : i=3; return eCodingSuccess;
		case L'4' : i=4; return eCodingSuccess;
		case L'5' : i=5; return eCodingSuccess;
		case L'6' : i=6; return eCodingSuccess;
		case L'7' : i=7; return eCodingSuccess;
		case L'8' : i=8; return eCodingSuccess;
		case L'9' : i=9; return eCodingSuccess;
		case L'a' : i=10; return eCodingSuccess;
		case L'b' : i=11; return eCodingSuccess;
		case L'c' : i=12; return eCodingSuccess;
		case L'd' : i=13; return eCodingSuccess;
		case L'e' : i=14; return eCodingSuccess;
		case L'f' : i=15; return eCodingSuccess;
		case L'A' : i=10; return eCodingSuccess;
		case L'B' : i=11; return eCodingSuccess;
		case L'C' : i=12; return eCodingSuccess;
		case L'D' : i=13; return eCodingSuccess;
		case L'E' : i=14; return eCodingSuccess;
		case L'F' : i=15; return eCodingSuccess;
		default : return eCodingFailure;
		}
	}

	static inline Chr16 uppercase(Chr16 c)
	{
		switch (c)
		{
		case L'a' : return L'A';
		case L'b' : return L'B';
		case L'c' : return L'C';
		case L'd' : return L'D';
		case L'e' : return L'E';
		case L'f' : return L'F';
		case L'g' : return L'G';
		case L'h' : return L'H';
		case L'i' : return L'I';
		case L'j' : return L'J';
		case L'k' : return L'K';
		case L'l' : return L'L';
		case L'm' : return L'M';
		case L'n' : return L'N';
		case L'o' : return L'O';
		case L'p' : return L'P';
		case L'q' : return L'Q';
		case L'r' : return L'R';
		case L's' : return L'S';
		case L't' : return L'T';
		case L'u' : return L'U';
		case L'v' : return L'V';
		case L'w' : return L'W';
		case L'x' : return L'X';
		case L'y' : return L'Y';
		case L'z' : return L'Z';
		default : return c;
		}
	}
};
