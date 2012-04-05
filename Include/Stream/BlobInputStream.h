// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Stream/MemoryInputStream.h"

class BlobInputStream : public MemoryInputStream
{
public:
	BlobInputStream(const UTF8& s)
		: m_blob(s.asBlob())
		, MemoryInputStream(s.ptr(), s.length())
	{}

	BlobInputStream(const Blob& b)
		: m_blob(b)
		, MemoryInputStream(b.ptr(), b.length())
	{}

	virtual ~BlobInputStream() {};

private:

	Blob m_blob;

private: BlobInputStream& operator=(const BlobInputStream&) {};
};
