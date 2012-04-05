// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Stream/OutputStream.h"

class MemoryOutputStream : public OutputStream
{
public:
	MemoryOutputStream(Buffer&);
	virtual ~MemoryOutputStream() {};

	virtual void write(Error&, const void* pBuf, Size iNumBytes);

private:

	Buffer& m_buf;

private: MemoryOutputStream& operator=(const MemoryOutputStream&) {};
};
