// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Context.h"

class MemoryContext : public Context
{
public:
	MemoryContext(const Context& context);
	~MemoryContext();

private:
	HBITMAP m_bm;
};