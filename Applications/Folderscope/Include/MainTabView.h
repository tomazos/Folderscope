// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "TabView.h"
#include "WelcomeView.h"

DeclWeakHandleable(MainTabView);
class MainTabView : public TabView
{
public:
	MainTabView(GUI* pParent);
	HWelcomeView getWelcomeView() { return m_hWelcomeView; }

	virtual void onPaneShown(HGUI pane);
	virtual void onPaneHidden(HGUI pane);

private:
	HWelcomeView m_hWelcomeView;
};
