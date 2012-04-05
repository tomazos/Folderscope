// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderImageView.h"

#include "Runtime.h"

#include "FontProvider.h"

#include "DrawingContext.h"
#include "PaintContext.h"
#include "OffscreenContext.h"
#include "MemoryContext.h"
#include "Controller.h"

FolderImageView::FolderImageView(GUI* pParent, HFolderImage image)
	: GUI(pParent, WS_CHILD | WS_CLIPCHILDREN)
	, m_image(image)
{
	m_workingView = NewObject(FolderWorkingView, this);
	m_idleView = NewObject(FolderIdleView, this);
	m_image->addListener(this);
}

FolderImageView::~FolderImageView()
{
	m_image->removeListener(this);
}

Result FolderImageView::msg_size(int iWidth, int iHeight)
{
	m_workingView->move(10, 10, iWidth - 20, iHeight - 20, true);
	m_idleView->move(10, 10, iWidth - 20, iHeight - 20, true);
	return eDefault;
};

Result FolderImageView::msg_paint()
{
	PaintContext pcontext(this);
	OffscreenContext context(&pcontext);

	context.setFont(Font::eStandard);

	context.rect(Brush::eActiveBorder, 0, 0, context.getWidth(), context.getHeight());

	context.blit();
	return eOk;
}

void FolderImageView::FolderImage_enteringState(FolderImage::EState eState)
{
	if (eState == FolderImage::eWorking)
	{
		m_workingView->show();
	}
	else if (eState == FolderImage::eIdle)
	{
		m_idleView->attachImage(m_image);
		m_idleView->show();
	}
	else if (eState == FolderImage::eShutdown)
	{
		Controller::get()->delImage(m_image);
	}
}

void FolderImageView::FolderImage_leavingState(FolderImage::EState eState)
{
	if (eState == FolderImage::eWorking)
	{
		m_workingView->hide();
	}
	else if (eState == FolderImage::eIdle)
	{
		m_idleView->hide();
	}
}

void FolderImageView::FolderImage_progress(const ProgressReport& report)
{
	m_workingView->setProgress(report);
	invalidate();
}

void FolderImageView::FolderImage_info(UTF16 sInfo)
{
	loginfo("%s", sInfo.ptr());
}

void FolderImageView::FolderImage_warning(UTF16 sError)
{
	logwarn("%s", sError.ptr());
}

void FolderImageView::FolderImage_fatal(UTF16 sError)
{
	logerr("%s", sError.ptr());
}

void FolderImageView::FolderImage_quickJobStart()
{
	m_workingView->disable();
	m_idleView->disable();
	setCursor(eWait);
	PostMessage(m_hwnd, WM_SETCURSOR, 0, 0);
}

void FolderImageView::FolderImage_quickJobEnd()
{
	setCursor(eArrow);
	m_workingView->enable();
	m_idleView->enable();
	setCursor(eArrow);
	redraw();
	PostMessage(m_hwnd, WM_SETCURSOR, 0, 0);
}
