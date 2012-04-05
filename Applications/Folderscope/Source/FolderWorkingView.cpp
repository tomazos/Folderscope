// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderWorkingView.h"

#include "Runtime.h"

#include "FontProvider.h"

#include "DrawingContext.h"
#include "PaintContext.h"
#include "OffscreenContext.h"
#include "MemoryContext.h"
#include "Controller.h"

FolderWorkingView::FolderWorkingView(GUI* pParent)
	: GUI(pParent, WS_CHILD | WS_CLIPCHILDREN)
{
	m_progressBar = NewObject(ProgressBar, this);
}

Result FolderWorkingView::msg_size(int iWidth, int iHeight)
{
	m_progressBar->move(10,10, iWidth - 20, 30, true);
	return eDefault;
};

Result FolderWorkingView::msg_paint()
{
	ProgressReport report;
	{
		Synchronizer sync(m_lock);
		report = m_report;
	}

	PaintContext pcontext(this);
	OffscreenContext context(&pcontext);

	context.rect(Brush::eActiveBorder, 0, 0, context.getWidth(), context.getHeight());

	context.setTransparentMode();
	context.setFont(Font::eBold);

	context.text(L"Task: ",10,60);
	context.text(L"Subtask: ",10,80);
	context.text(L"Start: ",10,100);
	context.text(L"ETA: ",10,120);
	context.text(L"Total: ",10,140);

	context.setFont(Font::eStandard);

	context.text(report.getTaskDescription(), 100, 60);
	context.text(report.getSubtaskDescription(), 100, 80);
	context.text(report.getRunningPeriod(), 100, 100);
	context.text(report.getCompletePeriod(), 100, 120);
	context.text(report.getTotalPeriod(), 100, 140);
		
	context.blit();
	return eOk;
}

void FolderWorkingView::setProgress(const ProgressReport& report)
{
	{
		Synchronizer sync(m_lock);
		m_report = report;
	}

	if (m_progressBar.isSet())
		m_progressBar->setProgress(report.getProgress());
	invalidate();
}
