// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/BufferedInputStream.h"

#include "Basics.h"
#include "Stream.h"

BufferedInputStream::BufferedInputStream(HInputStream hSource, Size iBufferSize)
	: m_hSource(hSource)
	, m_iBufferSize(iBufferSize)
	, m_buf(iBufferSize)
{
	m_pBufferStart = (UInt8*) m_buf.ptr();
	m_pBufferEnd = m_pBufferStart + iBufferSize;
	m_pDataStart = m_pBufferStart;
	m_pDataEnd = m_pDataStart;
}

BufferedInputStream::~BufferedInputStream()
{

}

Size BufferedInputStream::read(Error& error, void* pOutputStart, Size iNumReadRequestBytes)
{
	Size iNumOriginalBufferBytes = m_pDataEnd - m_pDataStart;

	if (iNumOriginalBufferBytes >= iNumReadRequestBytes)
	{
		Memory::copy(m_pDataStart, pOutputStart, iNumReadRequestBytes);
		m_pDataStart += iNumReadRequestBytes;
		return iNumReadRequestBytes;
	}
	else
	{
		Memory::copy(m_pDataStart, pOutputStart, iNumOriginalBufferBytes);
		Size iNumReadRemainingBytes = iNumReadRequestBytes - iNumOriginalBufferBytes;
		UInt8* pOutputNext = ((UInt8*)pOutputStart) + iNumOriginalBufferBytes;

		if (iNumReadRemainingBytes > m_iBufferSize)
			return iNumOriginalBufferBytes + m_hSource->read(error, pOutputNext, iNumReadRemainingBytes);

		Size iNumSourceReadBytes = m_hSource->read(error, m_pBufferStart, m_iBufferSize);
		
		if (error)
			return 0;

		m_pDataStart = m_pBufferStart;
		m_pDataEnd = m_pBufferStart + iNumSourceReadBytes;

		if (iNumSourceReadBytes >= iNumReadRemainingBytes)
		{
			Memory::copy(m_pDataStart, pOutputNext, iNumReadRemainingBytes);
			m_pDataStart += iNumReadRemainingBytes;
			return iNumReadRequestBytes;
		}
		else
		{
			Memory::copy(m_pDataStart, pOutputNext, iNumSourceReadBytes);
			m_pDataStart += iNumSourceReadBytes;
			return iNumOriginalBufferBytes + iNumSourceReadBytes;
		}
	}
}
