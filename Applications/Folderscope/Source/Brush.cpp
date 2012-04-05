// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Brush.h"

Brush::Brush(EBrushName eBrushName)
{
	int nIndex = 0;
	switch (eBrushName)
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

	m_brush = GetSysColorBrush(nIndex);
}

HBRUSH Brush::get_HBRUSH() const
{
	return m_brush;
}