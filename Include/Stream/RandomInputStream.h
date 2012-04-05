// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Stream/InputStream.h"

DeclHandleable(RandomInputStream);
class RandomInputStream : public InputStream
{
public:
	RandomInputStream(Error&);
	virtual ~RandomInputStream();

	virtual Size read(Error&, void* pBuf, Size iNumBytes);

private:
	
	ULONG_PTR m_hProv;

private: RandomInputStream& operator=(const RandomInputStream&) {};
};
