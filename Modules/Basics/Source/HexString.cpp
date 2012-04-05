// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/HexString.h"

#include "Basics/Buffer.h"

Character::ECodingResult
HexString::hexToBin(const Ascii& sHex, Blob& out)
{
	static UInt8* pTable(NULL);

	static const Chr8* sHexLookupUppercase("0123456789ABCDEF");
	static const Chr8* sHexLookupLowercase("0123456789abcdef");

	if (pTable == NULL) 
	{
		pTable = (UInt8*) NewBytes(256);
		for (int i = 0; i < 256; i++)
			pTable[i] = 0xFF;

		for (const Chr8* p = sHexLookupUppercase; p < sHexLookupUppercase + 16; p++)
			pTable[*p] = p - sHexLookupUppercase;

		for (const Chr8* p = sHexLookupLowercase; p < sHexLookupLowercase + 16; p++)
			pTable[*p] = p - sHexLookupLowercase;
	}

	if (sHex.length() % 2 != 0)
		return Character::eCodingFailure;

	const UInt8* pInput = (UInt8*) sHex.ptr();

	Buffer buf(sHex.length() / 2);
	UInt8* pOutput = (UInt8*) buf.ptr();

	for (int i = 0; i < sHex.length(); i+=2)
	{
		UInt8 hi = pTable[pInput[i]];
		UInt8 lo = pTable[pInput[i+1]];
		if (hi == 0xFF || lo == 0xFF)
			return Character::eCodingFailure;
		
		pOutput[i/2] = (hi << 4) + lo;
	}

	out = Blob(buf);
	return Character::eCodingSuccess;
}

Ascii HexString::binToHex(const Blob& b)
{
	static const Chr8* sHexLookup("0123456789ABCDEF");

	Chr8* pOutput = (Chr8*) NewBytes(b.length()*2+1);
	pOutput[b.length()*2] = 0;

	const UInt8* pInput = (UInt8*) b.ptr();

	for (int i = 0; i < b.length(); i++)
	{
		pOutput[2*i] = sHexLookup[ (pInput[i] & 0xF0) >> 4 ];
		pOutput[2*i+1] = sHexLookup[ (pInput[i] & 0x0F) ];
	}

	return Ascii(Blob::own(pOutput,b.length()*2+1));
}
