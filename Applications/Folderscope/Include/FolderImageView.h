// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "GUI.h"
#include "FolderWorkingView.h"
#include "FolderIdleView.h"
#include "FolderImage.h"

DeclWeakHandleable(FolderImageView);
class FolderImageView : public GUI, public FolderImage::Listener
{
public:
	FolderImageView(GUI* pParent, HFolderImage);
	virtual ~FolderImageView();

	virtual Result msg_size(int iWidth, int iHeight);
	virtual Result msg_paint();
	
	void registerWindow();

	virtual void FolderImage_enteringState(FolderImage::EState);
	virtual void FolderImage_leavingState(FolderImage::EState);
	virtual void FolderImage_progress(const ProgressReport& report);
	virtual void FolderImage_info(UTF16 sInfo);
	virtual void FolderImage_warning(UTF16 sError);
	virtual void FolderImage_fatal(UTF16 sError);
	virtual void FolderImage_quickJobStart();
	virtual void FolderImage_quickJobEnd();

	HFolderImage getImage() { return m_image; }

private:
	HFolderImage m_image;
	HFolderWorkingView m_workingView;
	HFolderIdleView m_idleView;
};
