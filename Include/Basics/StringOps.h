// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Buffer.h"

template<class T>
class StringOps
{
public:
	static bool startsWith(const T& sSubject, const T& sPrefix)
	{
		if (sPrefix.length() > sSubject.length())
			return false;

		return Memory::equal(sSubject.ptr(), sPrefix.ptr(), sizeof(T::CodeUnit) * sPrefix.length());
	}

	static bool endsWith(const T& sSubject, const T& sSuffix)
	{
		if (sSuffix.length() > sSubject.length())
			return false;

		return Memory::equal(sSuffix.ptr(), sSubject.ptr() + sSubject.length() - sSuffix.length(),
			sizeof(T::CodeUnit) * sSuffix.length());
	}

	static bool contains(const T& sSubject, const T& sSubstring)  
	{
		if (sSubstring.length() == 0)
			return true;

		if (sSubstring.length() > sSubject.length())
			return false;

		const T::CodeUnit* ss = sSubstring.ptr();
		Size iByteLength = sSubstring.length() * sizeof(T::CodeUnit);

		const T::CodeUnit* end = sSubject.ptr() + sSubject.length() - sSubstring.length() + 1;

		for (const T::CodeUnit* b = sSubject.ptr(); b != end; b++)
			if (Memory::equal(ss, b, iByteLength))
				return true;

		return false;
	}

	static T prefix(const T& sSubject, Size iNumCodeUnits)
	{
		if (iNumCodeUnits >= sSubject.length())
			return sSubject;

		if (iNumCodeUnits <= 0)
			return T();

		Buffer buf((iNumCodeUnits+1) * sizeof(T::CodeUnit));

		buf.idx<T::CodeUnit>(iNumCodeUnits) = T::CodeUnit(0);

		Memory::copy(sSubject.ptr(), buf.ptr(), iNumCodeUnits * sizeof(T::CodeUnit));

		return T(Blob(buf));
	}

	static T suffix(const T& sSubject, Size iNumCodeUnits)
	{
		if (iNumCodeUnits >= sSubject.length())
			return sSubject;

		if (iNumCodeUnits <= 0)
			return T();

		const T::CodeUnit* p = sSubject.ptr();

		p += sSubject.length() - iNumCodeUnits;

		return T(p);
	}

	static T substring(const T& sSubject, Size iStartCodeUnit, Size iNumCodeUnits)
	{
		return prefix(suffix(sSubject, sSubject.length() - iStartCodeUnit),iNumCodeUnits);	
	}

	static T concat(const T& a, const T& b)
	{
		Buffer buf((a.length() + b.length() + 1) * sizeof(T::CodeUnit));

		Memory::copy(a.ptr(), buf.ptr(), a.length() * sizeof(T::CodeUnit));
		
		void* bdest = &(buf.idx<T::CodeUnit>(a.length()));
		
		Memory::copy(b.ptr(), bdest, (b.length() + 1) * sizeof(T::CodeUnit));

		return T(Blob(buf));
	}
};
