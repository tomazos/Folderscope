// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"

class FolderIdleView;

DeclWeakHandleable(FolderListBodyView);
class FolderListBodyView : public GUI
{
public:
	FolderListBodyView(FolderIdleView* pParent);

	virtual Result msg_paint();
	
	virtual Result msg_lbuttonup(int iX, int iY);
	virtual Result msg_lbuttondown(int iX, int iY);
	virtual Result msg_mousemove(int iX, int iY);
	virtual Result msg_mousehover(int iX, int iY);
	virtual Result msg_mouseleave();

	virtual Result vscroll_bottom();
	virtual Result vscroll_endscroll();
	virtual Result vscroll_linedown();
	virtual Result vscroll_lineup();
	virtual Result vscroll_pagedown();
	virtual Result vscroll_pageup();
	virtual Result vscroll_thumbposition(UInt16 iPos);
	virtual Result vscroll_thumbtrack(UInt16 iPos);
	virtual Result vscroll_top();

private:
	void onMouseEnter() {}
	void onMouseLeave() {}

	void onLButtonClick(int iDownX, int iDownY, int iUpX, int iUpY);

	void onHoverEnter(int iX, int iY);
	void onHoverLeave(int iX, int iY);

	bool m_bMouseInside;

	bool m_bMouseHovering;
	int m_iMouseHoveringX, m_iMouseHoveringY;

	bool m_bLButtonDownInside;
	int m_iLButtonDownX, m_iLButtonDownY;

	friend class FolderIdleView;
	FolderIdleView* m_pParent;
};
