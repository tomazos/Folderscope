// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Stream/OutputStream.h"

class BufferedOutputStream : public OutputStream
{
public:
	BufferedOutputStream(HOutputStream hDestination, Size iBufferSize);
	virtual ~BufferedOutputStream();

	virtual void write(Error&, const void* pBuf, Size iNumBytes);

	void flush(Error&);

private:

	HOutputStream m_hDestination;

	Size m_iBufferSize;
	Buffer m_buf;

	UInt8* m_pBufferStart;
	UInt8* m_pBufferEnd;
	UInt8* m_pDataStart;
	UInt8* m_pDataEnd;

private: BufferedOutputStream& operator=(const BufferedOutputStream&) {};
};
