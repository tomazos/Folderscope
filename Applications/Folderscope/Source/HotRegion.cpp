// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "HotRegion.h"

#include "FontProvider.h"
#include "PaintContext.h"

HotRegion::HotRegion(GUI* pParent)
	: GUI(pParent, WS_CHILD | WS_VISIBLE, eHand)
	, m_bMouseInside(false)
	, m_bLButtonDownInside(false)
{
}

HotRegion::~HotRegion()
{

}

Result HotRegion::msg_lbuttonup(int iX, int iY)
{
	bool bOnClick = false;

	if (m_bLButtonDownInside)
		bOnClick = true;
	
	m_bLButtonDownInside = false;

	if (bOnClick)
		onClick();

	return eOk;
}

Result HotRegion::msg_lbuttondown(int iX, int iY)
{
	m_bLButtonDownInside = true;

	return eOk;
}

Result HotRegion::msg_mousemove(int iX, int iY)
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

	return eOk;
}

Result HotRegion::msg_mouseleave()
{
	if (m_bMouseInside)
	{
		m_bMouseInside = false;
		m_bLButtonDownInside = false;
		onMouseLeave();
	}

	return eOk;
}

Result HotRegion::msg_paint()
{
	return eOk;
}
