// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Brush.h"
#include "Font.h"
#include "Color.h"

class Context
{
public:
	int getWidth() const;
	int getHeight() const;
	HDC createMemoryContext() const;
	HBITMAP createBitmap(int iWidth, int iHeight) const;

	void rect(const Brush& brush, int iLeft, int iTop, int iWidth, int iHeight);
	void triangle(const Brush& brush, int iX1, int iY1, int iX2, int iY2, int iX3, int iY3);

	void setFont(const Font&);
	void setTextColor(const Color&);
	void setTextAlignLeft();
	void setTextAlignRight();
	void setTextAlignCenter();
	void text(const UTF16& sString, int iLeft, int iTop);

	void calcTextSize(const UTF16& sString, int& iWidth, int& iHeight) const;
	int getTextWidth(const Font& font, const UTF16& s);
	int getTextHeight(const Font& font, const UTF16& s);

	void setOpaqueMode();
	void setTransparentMode();

protected:
	HGDIOBJ select(HGDIOBJ object);

	int m_iHeight;
	int m_iWidth;
	HDC m_hdc;

	friend class OffscreenContext;
	friend class DrawingContext;
	friend class PaintContext;
};
