// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "MemoryContext.h"

MemoryContext::MemoryContext(const Context& context)
{
	m_iHeight = context.getHeight();
	m_iWidth = context.getWidth();
	m_hdc = context.createMemoryContext();
	m_bm = context.createBitmap(m_iWidth, m_iHeight);
	select(m_bm);
};

MemoryContext::~MemoryContext()
{
	DeleteObject(m_bm);
	DeleteDC(m_hdc);
}