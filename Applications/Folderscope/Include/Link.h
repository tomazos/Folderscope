// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Runtime.h"

#include "GUI.h"

DeclWeakHandleable(Link);
class Link : public GUI
{
public:
	Link(GUI* pParent, const UTF16& sText);
	virtual ~Link();

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
	void registerWindow();

	bool m_bMouseInside;
	bool m_bLButtonDownInside;
};
