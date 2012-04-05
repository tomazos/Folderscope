// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "OffscreenContext.h"

OffscreenContext::OffscreenContext(Context* pContext)
	: m_pContext(pContext)
{
	m_hdc = CreateCompatibleDC(m_pContext->m_hdc);
	m_bmp = CreateCompatibleBitmap(m_pContext->m_hdc, m_pContext->getWidth(), m_pContext->getHeight());
	select(m_bmp);

	m_iWidth = m_pContext->getWidth();
	m_iHeight = m_pContext->getHeight();
	
	SetBkMode(m_hdc, TRANSPARENT);
}

void OffscreenContext::blit()
{
	BitBlt(m_pContext->m_hdc, 0, 0, getWidth(), getHeight(), m_hdc, 0, 0, SRCCOPY);
}

OffscreenContext::~OffscreenContext()
{
	DeleteDC(m_hdc);
	DeleteObject(m_bmp);
}
