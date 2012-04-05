// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Stream/InputStream.h"

class BufferedInputStream : public InputStream
{
public:
	BufferedInputStream(HInputStream hSource, Size iBufferSize);
	virtual ~BufferedInputStream();

	virtual Size read(Error&, void* pBuf, Size iNumBytes);

private:

	Buffer m_buf;
	Size m_iBufferSize;

	UInt8* m_pBufferStart;
	UInt8* m_pBufferEnd;
	UInt8* m_pDataStart;
	UInt8* m_pDataEnd;
	HInputStream m_hSource;

private: BufferedInputStream& operator=(const BufferedInputStream&) {};
};
