// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"

#include "Basics.h"

#include "FolderSuggestionProvider.h"

class WelcomeView;

DeclWeakHandleable(FolderAssist);
class FolderAssist : public GUI, FolderSuggestionProvider::Listener
{
public:
	void registerWindow();
	FolderAssist(WelcomeView* pParent, int iUnit);
	virtual ~FolderAssist();

	virtual Result msg_size(int iWidth, int iHeight);
	virtual Result msg_paint();

	Array<FolderSuggestion> m_suggestions;

	void setPrefix(const UTF16& sPrefix);
	void dirty();

	Size m_iLineHeight;
	Size m_iNumLines;
	UTF16 m_sPrefix;
	Lock m_lock;
	Size m_iUnit;

	void FolderAssist::async_GetSuggestions();

	bool m_bDirty;

	virtual void notifySuggestions(HFolderSuggestionList);

	HFolderSuggestionProvider m_sugprov;

	int m_iSelected;

	void onDownArrow();
	void onUpArrow();
	void onEnterKey();

	WelcomeView* m_pWelcomeView;
};
