// (C) 2009 Tomazos-Jager Software Labs
#pragma once
#include "global.h"

#include "Basics/Memory.h"
#include "Basics/Blob.h"

class Buffer
{
public:
	inline Buffer(Size iNumBytes);
	inline Buffer(const Buffer&);
	inline Buffer& operator=(const Buffer&);

	inline ~Buffer();

	inline Size length();

	inline operator Blob();

	inline void resize(Size iNumBytes);

	inline void* ptr();

	template <class T>
	T& idx(Size i) { return ((T*)ptr())[i]; }

	template <class C>
	void trimString();

private:
	inline void close();
	Size m_iSize;
	void* m_pBuf;
};

inline void Buffer::close()
{
	if (m_pBuf)
	{
		DelBytes(m_pBuf);
		m_pBuf = NULL;
	}
}

Buffer::Buffer(Size iNumBytes)
	: m_pBuf(NewBytes(iNumBytes))
	, m_iSize(iNumBytes)
{
	
}

Buffer::Buffer(const Buffer& that)
	: m_pBuf(NewBytes(that.m_iSize))
	, m_iSize(that.m_iSize)
{
	Memory::copy(that.m_pBuf, m_pBuf, m_iSize);
}

inline Buffer& Buffer::operator=(const Buffer& that)
{
	close();

	m_iSize = that.m_iSize;
	m_pBuf = NewBytes(m_iSize);
	Memory::copy(that.m_pBuf, m_pBuf, m_iSize);
}

Buffer::operator Blob()
{
	void* pBuf = m_pBuf;
	Size iSize = m_iSize;
	m_pBuf = NewBytes(0);
	m_iSize = 0;
	return Blob::own(pBuf,iSize);
}

Buffer::~Buffer()
{
	close();
}

Size Buffer::length()
{
	return m_iSize;
}

void
Buffer::resize(Size iNumBytes)
{
	void* pNewBuf = NewBytes(iNumBytes);
	Memory::copy(m_pBuf, pNewBuf, min(iNumBytes,m_iSize));
	m_iSize = iNumBytes;
	DelBytes(m_pBuf);
	m_pBuf = pNewBuf;
}

void*
Buffer::ptr()
{
	return m_pBuf;
}

template <class C>
void Buffer::trimString()
{
	C* ps = (C*) ptr();
	C* pe = ps;
	while ((*pe) != C(0))
		pe++;

	resize((pe + 1 - ps)*sizeof(C));
}
