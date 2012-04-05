// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"

#include "FIItem.h"

class FolderIdleView;

DeclWeakHandleable(FolderListHeaderView);
class FolderListHeaderView : public GUI
{
public:
	FolderListHeaderView(FolderIdleView* pParent);

	virtual Result msg_paint();

private:

	FolderIdleView* m_pParent;
};
