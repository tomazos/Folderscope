// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

template <class T>
class DefaultComparator
{
public:
	static inline SInt64 comparef(const T& a, const T& b)
	{
		if (a == b)
			return 0;

		if (a < b)
			return -1;

		return 1;
	};
};

#define DeclComparable(X) \
	template<> \
	class DefaultComparator<X> \
	{ \
	public: \
		static inline UInt64 comparef(const X& a, const X& b) \
		{ \
			return X::comparef(a,b); \
		}; \
	};
