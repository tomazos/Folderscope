// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"
#include "ProgressBar.h"
#include "FolderImage.h"

DeclWeakHandleable(FolderWorkingView);
class FolderWorkingView : public GUI
{
public:
	FolderWorkingView(GUI* pParent);

	virtual Result msg_size(int iWidth, int iHeight);
	virtual Result msg_paint();

	void setProgress(const ProgressReport& report);

private:
	Lock m_lock;

	ProgressReport m_report;
	HProgressBar m_progressBar;
};
