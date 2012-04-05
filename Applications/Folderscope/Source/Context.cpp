// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Context.h"	

#include "Runtime.h"

int Context::getWidth() const
{
	return m_iWidth;
}

int Context::getHeight() const
{
	return m_iHeight;
}

void Context::rect(const Brush& brush, int iLeft, int iTop, int iWidth, int iHeight)
{
	RECT r;
	r.top = iTop;
	r.bottom = iTop + iHeight;
	r.left = iLeft;
	r.right = iLeft + iWidth;
	FillRect(m_hdc, &r, brush.get_HBRUSH());
}

void Context::triangle(const Brush& brush, int iX1, int iY1, int iX2, int iY2, int iX3, int iY3)
{
	POINT pts[3];
	pts[0].x = iX1;
	pts[0].y = iY1;
	pts[1].x = iX2;
	pts[1].y = iY2;
	pts[2].x = iX3;
	pts[2].y = iY3;

	HGDIOBJ oldbr = select(brush.get_HBRUSH());
	Check (oldbr != NULL);

	Polygon(m_hdc, pts, 3);

	select(oldbr);
}

HGDIOBJ Context::select(HGDIOBJ object)
{
	return SelectObject(m_hdc, object);
}

void Context::text(const UTF16& sString, int iLeft, int iTop)
{
	BOOL res = TextOut(m_hdc, iLeft, iTop, sString.ptr(), sString.length());

	Check(res != 0);
}

void Context::calcTextSize(const UTF16& sString, int& iWidth, int& iHeight) const
{
	SIZE s;
	BOOL res = GetTextExtentPoint32(m_hdc, sString.ptr(), sString.length(), &s);
	iHeight = s.cy;
	iWidth = s.cx;

	Check(res != 0);
};

HDC Context::createMemoryContext() const
{
	return CreateCompatibleDC(m_hdc);
}

HBITMAP Context::createBitmap(int iWidth, int iHeight) const
{
	return CreateCompatibleBitmap(m_hdc, iWidth, iHeight);
}

void Context::setTextColor(const Color& color)
{
	SetTextColor(m_hdc, color.get_COLORREF());
}

void Context::setTextAlignLeft()
{
	UINT i = GetTextAlign(m_hdc);
	i &= ~TA_LEFT;
	i &= ~TA_RIGHT;
	i &= ~TA_CENTER;
	i |= TA_LEFT;
	SetTextAlign (m_hdc, i);
}

void Context::setTextAlignRight()
{
	UINT i = GetTextAlign(m_hdc);
	i &= ~TA_LEFT;
	i &= ~TA_RIGHT;
	i &= ~TA_CENTER;
	i |= TA_RIGHT;
	SetTextAlign (m_hdc, i);
}

void Context::setTextAlignCenter()
{
	UINT i = GetTextAlign(m_hdc);
	i &= ~TA_LEFT;
	i &= ~TA_RIGHT;
	i &= ~TA_CENTER;
	i |= TA_CENTER;
	SetTextAlign (m_hdc, i);
}

void Context::setOpaqueMode()
{
	SetBkMode(m_hdc, OPAQUE);
};

void Context::setTransparentMode()
{
	SetBkMode(m_hdc, TRANSPARENT);
};

int Context::getTextHeight(const Font& font, const UTF16& s)
{
	HFONT oldFont = (HFONT) select(font.get_HFONT());
	int iHeight, iWidth;
	calcTextSize(s, iWidth, iHeight);
	select(oldFont);
	return iHeight;
}

int Context::getTextWidth(const Font& font, const UTF16& s)
{
	HFONT oldFont = (HFONT) select(font.get_HFONT());
	int iHeight, iWidth;
	calcTextSize(s, iWidth, iHeight);
	select(oldFont);
	return iWidth;
}

void Context::setFont(const Font& font)
{
	select(font.get_HFONT());
}