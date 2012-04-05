// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class SHA512
{
public:
	SHA512();

	void write(const void* pBuf, Size iByteLen);
	Blob finish();

private:
	void init();
	void last();
	void transform(const UInt64*);

	UInt64	state[8];
	UInt64	bitcount[2];
	UInt8	buffer[128];
};
