// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderListHeaderView.h"

#include "FolderIdleView.h"

#include "PaintContext.h"

FolderListHeaderView::FolderListHeaderView(FolderIdleView* pParent)
	: m_pParent(pParent)
	, GUI(pParent, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE)
{}

Result FolderListHeaderView::msg_paint()
{
	PaintContext pcontext(this);
	OffscreenContext context(&pcontext);

	m_pParent->drawHeader(context);

	context.blit();
	return eOk;
}

