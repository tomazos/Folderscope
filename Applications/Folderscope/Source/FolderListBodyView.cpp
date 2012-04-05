// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderListBodyView.h"

#include "FolderIdleView.h"

#include "PaintContext.h"
#include "OffscreenContext.h"

#include "Runtime.h"


FolderListBodyView::FolderListBodyView(FolderIdleView* pParent)
	: GUI(pParent, WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_VISIBLE)
	, m_pParent(pParent)
	, m_bMouseInside(false)
    , m_bMouseHovering(false)
	, m_iMouseHoveringX(0)
	, m_iMouseHoveringY(0)
    , m_bLButtonDownInside(false)
	, m_iLButtonDownX(0)
	, m_iLButtonDownY(0)
{}

Result FolderListBodyView::msg_paint()
{
	PaintContext pcontext(this);
	OffscreenContext context(&pcontext);

	m_pParent->drawBody(context);

	context.blit();
	return eOk;
}

Result FolderListBodyView::vscroll_bottom()
{
	return m_pParent->body_vscroll_bottom();
}

Result FolderListBodyView::vscroll_endscroll()
{
	return m_pParent->body_vscroll_endscroll();
}

Result FolderListBodyView::vscroll_linedown()
{
	return m_pParent->body_vscroll_linedown();
}

Result FolderListBodyView::vscroll_lineup()
{
	return m_pParent->body_vscroll_lineup();
}

Result FolderListBodyView::vscroll_pagedown()
{
	return m_pParent->body_vscroll_pagedown();
}

Result FolderListBodyView::vscroll_pageup()
{
	return m_pParent->body_vscroll_pageup();
}

Result FolderListBodyView::vscroll_thumbposition(UInt16 iPos)
{
	return m_pParent->body_vscroll_thumbposition(iPos);
}

Result FolderListBodyView::vscroll_thumbtrack(UInt16 iPos)
{
	return m_pParent->body_vscroll_thumbtrack(iPos);
}

Result FolderListBodyView::vscroll_top()
{
	return m_pParent->body_vscroll_top();
}

Result FolderListBodyView::msg_mousemove(int iX, int iY)
{
	if (!m_bMouseInside)
	{
		m_bMouseInside = true;
		onMouseEnter();

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = m_hwnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&tme);
	}
	
	if (m_bMouseHovering)
	{
		m_bMouseHovering = false;
		onHoverLeave(m_iMouseHoveringX, m_iMouseHoveringY);

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = m_hwnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&tme);
	}

	return eDefault;
}

Result FolderListBodyView::msg_mouseleave()
{
	if (m_bMouseInside)
	{
		m_bMouseInside = false;
		m_bMouseHovering = false;
		m_bLButtonDownInside = false;
		onMouseLeave();
	}

	return eOk;
}

Result FolderListBodyView::msg_lbuttondown(int iX, int iY)
{
	Check(m_bMouseInside);

	m_bLButtonDownInside = true;
	m_iLButtonDownX = iX;
	m_iLButtonDownY = iY;

	return eOk;
}

Result FolderListBodyView::msg_lbuttonup(int iX, int iY)
{
	if (m_bLButtonDownInside)
	{
		m_bLButtonDownInside = false;

		onLButtonClick(m_iLButtonDownX, m_iLButtonDownY, iX, iY);
	}

	return eOk;
}

Result FolderListBodyView::msg_mousehover(int iX, int iY)
{
	Check(!m_bMouseHovering);

	m_bMouseHovering = true;
	m_iMouseHoveringX = iX;
	m_iMouseHoveringY = iY;

	onHoverEnter(iX, iY);

	return eOk;
}


void FolderListBodyView::onHoverEnter(int iX, int iY)
{
	m_pParent->showHoverTipFor(iX, iY);
}

void FolderListBodyView::onHoverLeave(int iX, int iY)
{
	m_pParent->hideHoverTip();
}

void FolderListBodyView::onLButtonClick(int iDownX, int iDownY, int iUpX, int iUpY)
{
	m_pParent->click(iDownX, iDownY, iUpX, iUpY);
}
