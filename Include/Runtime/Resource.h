// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

class Resource
{
public:
	Resource();

	Size length() const;
	const void* ptr() const;

	template <class T>
	T idx(Size i) const { Check(i >= 0 && i < m_iNumBytes); return ((const T*)ptr())[i]; }

private:
	Resource(const void* pData, Size iNumBytes);
	friend class ResourceManager;

	Size m_iNumBytes;
	const void* m_pData;
};
