// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "PaintContext.h"

PaintContext::PaintContext(GUI* pWindow)
{
	m_hwnd = pWindow->m_hwnd;
	RECT r;
	GetClientRect(m_hwnd, &r);
	m_iHeight = r.bottom;
	m_iWidth = r.right;
	m_hdc = BeginPaint(m_hwnd, &m_ps);
	SetBkMode(m_hdc, TRANSPARENT);
}

PaintContext::~PaintContext()
{
	EndPaint(m_hwnd, &m_ps);
}