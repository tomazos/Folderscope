// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Context.h"

class OffscreenContext: public Context
{
public:
	OffscreenContext(Context* pContext);
	void blit();
	~OffscreenContext();

private:
	Context* m_pContext;
	HBITMAP m_bmp;
};