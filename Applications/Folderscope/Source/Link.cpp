// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Link.h"

#include "FontProvider.h"
#include "PaintContext.h"

Link::Link(GUI* pParent, const UTF16& sText)
	: GUI(pParent, WS_CHILD | WS_VISIBLE, eHand)
	, m_bMouseInside(false)
	, m_bLButtonDownInside(false)
{
	setText(sText);
}

Link::~Link()
{

}

Result Link::msg_lbuttonup(int iX, int iY)
{
	bool bOnClick = false;

	if (m_bLButtonDownInside)
		bOnClick = true;
	
	m_bLButtonDownInside = false;

	if (bOnClick)
		onClick();

	return eOk;
}

Result Link::msg_lbuttondown(int iX, int iY)
{
	m_bLButtonDownInside = true;

	return eOk;
}

Result Link::msg_mousemove(int iX, int iY)
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

Result Link::msg_mouseleave()
{
	if (m_bMouseInside)
	{
		m_bMouseInside = false;
		m_bLButtonDownInside = false;
		onMouseLeave();
	}

	return eOk;
}

Result Link::msg_paint()
{
	PaintContext context(this);
	context.setFont(Font::eStandard);

	UTF16 sText = getText();

	int iBoxWidth = context.getWidth();
	int iBoxHeight = context.getHeight();


	int iTextWidth = 0;
	int iTextHeight = 0;
	context.calcTextSize(sText, iTextWidth, iTextHeight);

	int iTextLeft = (iBoxWidth - iTextWidth) / 2;
	int iTextTop = (iBoxHeight - iTextHeight) / 2 - 1;

	context.setFont(Font::eStandard);

	context.rect(Brush::eActiveBorder, 0, 0, context.getWidth(), context.getHeight());

	context.setTextColor(Color::eHotlight);
	context.text(getText(), iTextLeft, iTextTop);
	context.rect(Brush::eHotlight, iTextLeft, iTextTop + iTextHeight + 1, iTextWidth, 1);

	return eOk;
}
