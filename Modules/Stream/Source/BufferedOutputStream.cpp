// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/BufferedOutputStream.h"

#include "Basics.h"
#include "Stream.h"

BufferedOutputStream::BufferedOutputStream(HOutputStream hDestination, Size iBufferSize)
	: m_hDestination(hDestination)
	, m_buf(iBufferSize)
	, m_iBufferSize(iBufferSize)
{
	m_pBufferStart = (UInt8*) m_buf.ptr();
	m_pBufferEnd = m_pBufferStart + iBufferSize;
	m_pDataStart = m_pBufferStart;
	m_pDataEnd = m_pDataStart;
}

BufferedOutputStream::~BufferedOutputStream()
{
	Error error;
	flush(error);
	Check(!error);
}

void
BufferedOutputStream::flush(Error& error)
{
	m_hDestination->write(error, m_pDataStart, m_pDataEnd - m_pDataStart);
	m_pDataStart = m_pBufferStart;
	m_pDataEnd = m_pDataStart;
}

void
BufferedOutputStream::write(Error& error, const void* pInputStart, Size iNumWriteRequestBytes)
{
	Size iRemainingSpace = m_pBufferEnd - m_pDataEnd;

	if (iNumWriteRequestBytes <= iRemainingSpace)
	{
		Memory::copy(pInputStart, m_pDataEnd, iNumWriteRequestBytes);
		m_pDataEnd += iNumWriteRequestBytes;
	}
	else
	{
		flush(error);

		if (error)
			return;

		if (iNumWriteRequestBytes > m_iBufferSize)
			m_hDestination->write(error, pInputStart, iNumWriteRequestBytes);
		else
		{
			Memory::copy(pInputStart, m_pDataEnd, iNumWriteRequestBytes);
			m_pDataEnd += iNumWriteRequestBytes;
		}
	}
}
