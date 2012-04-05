// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

class Color
{
public:
	Color(Real64 fRed, Real64 fGreen, Real64 fBlue);

	enum EColorName
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

	Color(EColorName eColorName);

private:
	Real64 m_fRed;
	Real64 m_fGreen;
	Real64 m_fBlue;

	friend Color operator+(Color, Color);
	friend Color operator-(Color, Color);
	friend Color operator*(Color, Real64);

	COLORREF get_COLORREF() const;

	friend class Context;
	friend class Brush;
};

Color operator+(Color, Color);
Color operator-(Color, Color);
Color operator*(Color, Real64);

inline Color operator*(Real64 k, Color a) { return a*k; }
inline Color operator/(Color a, Real64 k) { return a*(Real64(1)/k); }
