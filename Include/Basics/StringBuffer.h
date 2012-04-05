// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Buffer.h"

template <class T>
class StringBuffer
{
public:
	typedef typename T::CodeUnit CodeUnit;

	StringBuffer(Size iNumChars)
		: buf(iNumChars * sizeof(CodeUnit))
	{
		if (iNumChars < sizeof(CodeUnit))
			buf.resize(sizeof(CodeUnit));

		buf.idx<CodeUnit>(0) = CodeUnit(0);
		buf.idx<CodeUnit>(iNumChars-1) = CodeUnit(0);
	}

	operator T()
	{
		trim();
		return T(Blob(buf));
	}

	CodeUnit& operator[] (Size iChar)
	{
		return buf.idx<CodeUnit>(iChar);
	}

	CodeUnit* ptr()
	{
		return (CodeUnit*) buf.ptr();
	}

	Size clength()
	{
		CodeUnit* p = ptr();

		while ((*p) != CodeUnit(0))
			p++;

		return p - ptr();
	}

	Size flength()
	{
		return buf.length() / sizeof(CodeUnit);
	}

	void resize(Size iNumChars)
	{
		if (iNumChars >= 1)
		{
			buf.resize(iNumChars * sizeof(CodeUnit));
			buf.idx<CodeUnit>(iNumChars-1) = CodeUnit(0);
		}
		else
		{
			buf.resize(sizeof(CodeUnit));
			buf.idx<CodeUnit>(0) = CodeUnit(0);
		}
	}

	void trim()
	{
		if (buf.length() < sizeof(CodeUnit))
			buf.resize(sizeof(CodeUnit));
		else
			buf.idx<CodeUnit>(buf.length() / sizeof(CodeUnit) - 1) = CodeUnit(0);

		buf.trimString<CodeUnit>();
	}

	StringBuffer<T>& operator <<(const T& t)
	{
		int icl = clength();
		int ncl = icl + t.length() + 1;
		if (ncl > flength())
			resize(ncl);
		Memory::copy(t.ptr(), (ptr() + icl), (t.length() + 1) * sizeof(CodeUnit));
		return (*this);
	}

	StringBuffer<T>& operator <<(CodeUnit cu)
	{
		int icl = clength();

		if (icl + 2 > flength())
			resize(icl + 2);
		(*this)[icl] = cu;
		(*this)[icl+1] = CodeUnit(0);
		return (*this);
	}

private:
	Buffer buf;
};

typedef StringBuffer<Ascii> AsciiBuffer;
typedef StringBuffer<UTF8> UTF8Buffer;
typedef StringBuffer<UTF16> UTF16Buffer;
