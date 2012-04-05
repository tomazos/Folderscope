// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "MainTabView.h"

#include "LogView.h"
#include "FolderImageView.h"
#include "Controller.h"

MainTabView::MainTabView(GUI* pParent)
	: TabView(pParent)
{
	m_hWelcomeView = NewObject(WelcomeView, this);
	pushPane(L"Welcome", m_hWelcomeView);

	HGUI logView = NewObject(LogView, this);
	pushPane(L"Activity Log", logView);
}

void MainTabView::onPaneShown(HGUI pane)
{
	HFolderImageView fiview = pane.cast<FolderImageView> ();

	if (fiview.isSet())
	{
		HFolderImage image = fiview->getImage();
		Controller::get()->focusImage(image);
	}
}

void MainTabView::onPaneHidden(HGUI pane)
{
	HFolderImageView fiview = pane.cast<FolderImageView> ();

	if (fiview.isSet())
	{
		HFolderImage image = fiview->getImage();
		Controller::get()->blurImage(image);
	}
}
