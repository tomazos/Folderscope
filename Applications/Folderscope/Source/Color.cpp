// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Color.h"

#include "Basics.h"

Color::Color(Real64 fRed, Real64 fGreen, Real64 fBlue)
	: m_fRed(fRed)
	, m_fGreen(fGreen)
	, m_fBlue(fBlue)
{}

Color::Color(EColorName eColorName)
	: m_fRed(0)
	, m_fGreen(0)
	, m_fBlue(0)
{
	int nIndex = 0;
	switch (eColorName)
	{
	case e3DDkShadow: nIndex = COLOR_3DDKSHADOW; break;
	case e3DFace: nIndex = COLOR_3DFACE; break;
	case e3DHighlight: nIndex = COLOR_3DHIGHLIGHT; break;
	case e3DHilight: nIndex = COLOR_3DHILIGHT; break;
	case e3DLight: nIndex = COLOR_3DLIGHT; break;
	case e3DShadow: nIndex = COLOR_3DSHADOW; break;
    case eActiveBorder: nIndex = COLOR_ACTIVEBORDER; break;
    case eActiveCaption: nIndex = COLOR_ACTIVECAPTION; break;
    case eAppWorkspace: nIndex = COLOR_APPWORKSPACE; break;
    case eBackground: nIndex = COLOR_BACKGROUND; break;
    case eBtnFace: nIndex = COLOR_BTNFACE; break;
    case eBtnHighlight: nIndex = COLOR_BTNHIGHLIGHT; break;
    case eBtnHilight: nIndex = COLOR_BTNHILIGHT; break;
    case eBtnShadow: nIndex = COLOR_BTNSHADOW; break;
    case eBtnText: nIndex = COLOR_BTNTEXT; break;
    case eCaptionText: nIndex = COLOR_CAPTIONTEXT; break;
    case eDesktop: nIndex = COLOR_DESKTOP; break;
    case eGradientActiveCaption: nIndex = COLOR_GRADIENTACTIVECAPTION; break;
    case eGradientInactiveCaption: nIndex = COLOR_GRADIENTINACTIVECAPTION; break;
    case eGrayText: nIndex = COLOR_GRAYTEXT; break;
    case eHighlight: nIndex = COLOR_HIGHLIGHT; break;
    case eHighlightText: nIndex = COLOR_HIGHLIGHTTEXT; break;
    case eHotlight: nIndex = COLOR_HOTLIGHT; break;
    case eInactiveBorder: nIndex = COLOR_INACTIVEBORDER; break;
    case eInactiveCaption: nIndex = COLOR_INACTIVECAPTION; break;
    case eInactiveCaptionText: nIndex = COLOR_INACTIVECAPTIONTEXT; break;
    case eInfoBk: nIndex = COLOR_INFOBK; break;
    case eInfoText: nIndex = COLOR_INFOTEXT; break;
    case eMenu: nIndex = COLOR_MENU; break;
    case eMenuHilight: nIndex = COLOR_MENUHILIGHT; break;
    case eMenuBar: nIndex = COLOR_MENUBAR; break;
    case eMenuText: nIndex = COLOR_MENUTEXT; break;
    case eScrollBar: nIndex = COLOR_SCROLLBAR; break;
    case eWindow: nIndex = COLOR_WINDOW; break;
    case eWindowFrame: nIndex = COLOR_WINDOWFRAME; break;
	case eWindowText: nIndex = COLOR_WINDOWTEXT; break;
	default: Check(); return;
	}

	DWORD iSysColor = GetSysColor(nIndex);

	UInt8 iRed = GetRValue(iSysColor);
	UInt8 iGreen = GetGValue(iSysColor);
	UInt8 iBlue = GetBValue(iSysColor);

	m_fRed = Real64(iRed) / Real64(255);
	m_fGreen = Real64(iGreen) / Real64(255);
	m_fBlue = Real64(iBlue) / Real64(255);
}

Color operator+(Color a, Color b)
{
	return Color(a.m_fRed + b.m_fRed, a.m_fGreen + b.m_fGreen, a.m_fBlue + b.m_fBlue);
}

Color operator-(Color a, Color b)
{
	return Color(a.m_fRed - b.m_fRed, a.m_fGreen - b.m_fGreen, a.m_fBlue - b.m_fBlue);
}

Color operator*(Color a, Real64 k)
{
	return Color(a.m_fRed * k, a.m_fGreen * k, a.m_fBlue * k);
}

COLORREF Color::get_COLORREF() const
{
	return RGB(UInt8(m_fRed*255),UInt8(m_fGreen*255),UInt8(m_fBlue*255));
}
