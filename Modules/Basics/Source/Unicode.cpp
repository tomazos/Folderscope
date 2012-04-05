// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Unicode.h"

#include "Basics/Blob.h"
#include "Basics/Buffer.h"
#include "Basics/UTF8.h"
#include "Basics/UTF16.h"

static inline bool isFirstSurrogate(UInt16 i)
{
	return UInt16(0xD800) == UInt16( i & UInt16(0xFC00) );
}

static inline UInt32 low10Bits(UInt16 i)
{
	return UInt32 ( i & UInt16(0x03FF) );
}

Character::ECodingResult
Unicode::UTF8_to_UTF16(const UTF8& sSource, UTF16& sDestination)
{
	const UInt8* pIn = (const UInt8*) sSource.ptr();
	const UInt8* pEnd = ((const UInt8*) sSource.ptr()) + sSource.length() + 1;

	Buffer outBuf(sSource.length() * 2 + 2);
	UInt16* pOutStart = (UInt16*) outBuf.ptr();
	UInt16* pOut = pOutStart;

	while ((pIn[0]) != 0)
	{
		UInt32 nextChar;
		if ((pIn[0] & 0x80) == 0)
		{
			if (pIn + 1 >= pEnd)
				return Character::eCodingFailure;

			nextChar = pIn[0];

			pIn++;
		}
		else if ((pIn[0] & 0xE0) == 0xC0)
		{
			if (pIn + 2 >= pEnd)
				return Character::eCodingFailure;

			nextChar  = UInt32( pIn[0] & 0x1F ) << 6;
			nextChar |= UInt32( pIn[1] & 0x3F ) << 0;
			
			pIn += 2;
		}
		else if ((pIn[0] & 0xF0) == 0xE0)
		{
			if (pIn + 3 >= pEnd)
				return Character::eCodingFailure;

			nextChar  = UInt32( pIn[0] & 0x0F ) << 12;
			nextChar |= UInt32( pIn[1] & 0x3F ) << 6;
			nextChar |= UInt32( pIn[2] & 0x3F ) << 0;

			pIn += 3;
		}
		else if ((pIn[0] & 0xF8) == 0xF0)
		{
			if (pIn + 4 >= pEnd)
				return Character::eCodingFailure;

			nextChar  = UInt32( pIn[0] & 0x07 ) << 18;
			nextChar |= UInt32( pIn[1] & 0x3F ) << 12;
			nextChar |= UInt32( pIn[2] & 0x3F ) << 6;
			nextChar |= UInt32( pIn[3] & 0x3F ) << 0;
			
			pIn += 4;
		}
		else
			return Character::eCodingFailure;

		if (nextChar >= UInt32 ( 0x00010000U ) )
		{
			UInt32 remainder = nextChar - 0x10000;
			pOut[0] = UInt16(0xD800) + UInt16( (remainder & 0x000FFC00U ) >> 10);
			pOut[1] = UInt16(0xDC00) + UInt16( (remainder & 0x000003FFU ) >> 0);
			pOut += 2;
		}
		else
		{
			pOut[0] = UInt16(nextChar);
			pOut++;
		}
	}

	pOut[0] = 0;
	pOut++;
	Size iByteLength = (pOut - pOutStart) * 2;
	outBuf.resize(iByteLength);
	sDestination = UTF16(Blob(outBuf));
	return Character::eCodingSuccess;
}

Character::ECodingResult
Unicode::UTF16_to_UTF8(const UTF16& sSource, UTF8& sDestination)
{
	const UInt16* pIn = (const UInt16*) sSource.ptr();
	const UInt16* pEnd = ((const UInt16*) sSource.ptr()) + sSource.length() + 1;

	int iBufLength = sSource.length()*3+1;
	Buffer outBuf(iBufLength);
	UInt8* pOutStart = (UInt8*) outBuf.ptr();
	UInt8* pOut = pOutStart;

	while ((pIn[0]) != 0)
	{
		UInt32 nextChar;
		if (isFirstSurrogate(pIn[0]))
		{
			nextChar = UInt32(0x00010000U) + (low10Bits(pIn[0]) << 10);
			pIn ++;
			if (pIn == pEnd)
				return Character::eCodingFailure;

			nextChar += low10Bits(pIn[0]);
			pIn++;
		}
		else
		{
			nextChar = UInt32 (pIn[0]);
			pIn++;
		}

		if (pIn == pEnd)
			return Character::eCodingFailure;

		if (nextChar < UInt32(0x00000080U) )
		{
			pOut[0] = UInt8(nextChar);
			pOut++;
		}
		else if (nextChar < UInt32(0x00000800U) )
		{
			pOut[0] = UInt8(0xC0) | UInt8( (nextChar & UInt32(0x000007C0U) ) >> 6 );
			pOut[1] = UInt8(0x80) | UInt8( (nextChar & UInt32(0x0000003FU) ) >> 0 );
			pOut += 2;
		}
		else if (nextChar < UInt32(0x00010000U) )
		{
			pOut[0] = UInt8(0xE0) | UInt8( (nextChar & UInt32(0x0000F000U) ) >> 12 );
			pOut[1] = UInt8(0x80) | UInt8( (nextChar & UInt32(0x00000FC0U) ) >> 6 );
			pOut[2] = UInt8(0x80) | UInt8( (nextChar & UInt32(0x0000003FU) ) >> 0 );
			pOut += 3;
		}
		else if (nextChar < UInt32(0x00110000U) )
		{
			pOut[0] = UInt8(0xF0) | UInt8( (nextChar & UInt32(0x001C0000U) ) >> 18 );
			pOut[1] = UInt8(0x80) | UInt8( (nextChar & UInt32(0x0003F000U) ) >> 12 );
			pOut[2] = UInt8(0x80) | UInt8( (nextChar & UInt32(0x00000FC0U) ) >> 6 );
			pOut[3] = UInt8(0x80) | UInt8( (nextChar & UInt32(0x0000003FU) ) >> 0 );
			pOut += 4;
		}
		else
			return Character::eCodingFailure;
	}

	pOut[0] = 0;
	pOut++;

	Size iByteLength = pOut - pOutStart;

	outBuf.resize(iByteLength);

	sDestination = UTF8(Blob(outBuf));

	return Character::eCodingSuccess;
};
