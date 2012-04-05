// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"
#include "GUI.h"

DeclHandleable(TabView);
class TabView : public GUI
{
public:
	TabView(GUI* pParent);

	Size getNumPanes();
	HGUI getCurrentPane();
	void setCurrentPane(HGUI);

	int getCurrentIndex();
	void setCurrentIndex(Size iIndex);

	HGUI getPane(Size iIndex);
	int getIndex(HGUI);

	void pushPane(UTF16 sTabName, HGUI);
	void insertPane(Size iIndex, UTF16 sTabName, HGUI);

	void delIndex(int iIndex);
	void delPane(HGUI);

	virtual Result msg_size(int iWidth, int iHeight);

	virtual Result note_selchange();

	virtual void onPaneShown(HGUI pane) {};
	virtual void onPaneHidden(HGUI pane) {};

private:
	void hideIndex(Size iIndex);
	void showIndex(Size iIndex);

	void setPaneRects();

	void showCurrentTab();

	Array<HGUI> m_panes;
	int m_iCurrentIndex;
};
