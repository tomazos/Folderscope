// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "DrawingObject.h"

#include "Color.h"

class Brush
{
public:
	enum EBrushName
	{
		e3DDkShadow,
		e3DFace,
		e3DHighlight,
		e3DHilight,
		e3DLight,
		e3DShadow,
		eActiveBorder,
		eActiveCaption,
		eAppWorkspace,
		eBackground,
		eBtnFace,
		eBtnHighlight,
		eBtnHilight,
		eBtnShadow,
		eBtnText,
		eCaptionText,
		eDesktop,
		eGradientActiveCaption,
		eGradientInactiveCaption,
		eGrayText,
		eHighlight,
		eHighlightText,
		eHotlight,
		eInactiveBorder,
		eInactiveCaption,
		eInactiveCaptionText,
		eInfoBk,
		eInfoText,
		eMenu,
		eMenuHilight,
		eMenuBar,
		eMenuText,
		eScrollBar,
		eWindow,
		eWindowFrame,
		eWindowText
	};

	Brush(EBrushName);
private:
	HBRUSH m_brush;
	HBRUSH get_HBRUSH() const;
	friend class Context;
};
