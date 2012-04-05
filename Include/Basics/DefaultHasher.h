// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

template<class T>
class DefaultHasher
{
public:
	static UInt64 hashf(const T& t)
	{
		int iBytes = sizeof(T);

		UInt64 h = 0xA4238B58310421EFULL;

		UInt8* pStart = (UInt8*) &t;
		UInt8* pEnd = pStart + iBytes;

		for (UInt8* p = pStart; p != pEnd; p++)
			h += ((h << 5) ^ UInt64(*p));

		return h;
	};
};

#define DeclHashable(X) \
	template<> \
	class DefaultHasher<X> \
	{ \
	public: \
		static UInt64 hashf(const X& t) \
		{ \
			return X::hashf(t); \
		}; \
	};
