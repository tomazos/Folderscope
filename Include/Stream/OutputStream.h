// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

DeclHandleable(OutputStream);

class OutputStream : public Handleable
{
public:
	virtual void write(Error&, const void* pBuf, Size iNumBytes) = 0;

	template <class T>
	   inline void twrite(Error&, const T&);
protected: OutputStream() {}
private: OutputStream(const OutputStream&) {} 
private: OutputStream& operator= (const OutputStream&) { return *this; } 
public: virtual ~OutputStream() {}
};

template <class T>
inline void plainWriteToStream(Error& error, OutputStream* out, const T& t)
{
	out->write(error, &t, sizeof(t));
}

inline void writeToStream(Error& error, OutputStream* out, UInt8 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, UInt16 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, UInt32 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, UInt64 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, SInt8 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, SInt16 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, SInt32 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, SInt64 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, Real32 t)
{
	plainWriteToStream(error, out, t);
};

inline void writeToStream(Error& error, OutputStream* out, Real64 t)
{
	plainWriteToStream(error, out, t);
};

//inline void writeToStream(Error& error, OutputStream* out, Chr8 t)
//{
//	plainWriteToStream(error, out, t);
//};

inline void writeToStream(Error& error, OutputStream* out, Chr16 t)
{
	plainWriteToStream(error, out, t);
};

template <class T>
inline void stringWriteToStream(Error& error, OutputStream* out, const T& t)
{
	out->write(error, t.ptr(), sizeof(T::CodeUnit) * t.length());
}

inline void writeToStream(Error& error, OutputStream* out, const Ascii& t)
{
	stringWriteToStream(error, out, t);
}

inline void writeToStream(Error& error, OutputStream* out, const UTF8& t)
{
	stringWriteToStream(error, out, t);
}

inline void writeToStream(Error& error, OutputStream* out, const UTF16& t)
{
	stringWriteToStream(error, out, t);
}

template <class T>
inline void OutputStream::twrite(Error& error, const T& t)
{
	writeToStream(error, this, t);
}
