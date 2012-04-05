// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Runtime.h"

#include "GUI.h"

DeclWeakHandleable(HotRegion);
class HotRegion : public GUI
{
public:
	HotRegion(GUI* pParent);
	virtual ~HotRegion();

protected:

	virtual void onClick() = 0;
	virtual void onMouseEnter() {};
	virtual void onMouseLeave() {};
	virtual Result msg_lbuttonup(int iX, int iY);
	virtual Result msg_lbuttondown(int iX, int iY);
	virtual Result msg_mousemove(int iX, int iY);
	virtual Result msg_mouseleave();
	virtual Result msg_paint();

private:
	bool m_bMouseInside;
	bool m_bLButtonDownInside;
};
