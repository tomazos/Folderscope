// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/Resource.h"

Resource::Resource()
	: m_iNumBytes(0)
	, m_pData(NULL)
{}

Resource::Resource(const void* pData, Size iNumBytes)
	: m_pData(pData)
	, m_iNumBytes(iNumBytes)
{}

Size Resource::length() const
{
	return m_iNumBytes;
}

const void* Resource::ptr() const
{
	return m_pData;
}
