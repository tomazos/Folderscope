// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "DrawingContext.h"

DrawingContext::DrawingContext(GUI* pWindow)
{
	m_hwnd = pWindow->m_hwnd;
	RECT r;
	int iRes = GetClientRect(m_hwnd, &r);
	Check(iRes != 0);
	m_iHeight = r.bottom;
	m_iWidth = r.right;
	m_hdc = GetDC(m_hwnd);
}

DrawingContext::~DrawingContext()
{
	ReleaseDC(m_hwnd, m_hdc);
}