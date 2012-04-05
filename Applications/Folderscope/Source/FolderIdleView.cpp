// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderIdleView.h"

#include "Runtime.h"

#include "FontProvider.h"

#include "DrawingContext.h"
#include "PaintContext.h"
#include "OffscreenContext.h"
#include "MemoryContext.h"
#include "Controller.h"

#include "FolderListHeaderView.h"
#include "FolderListBodyView.h"

#include "HotRegion.h"

class SorterButton : public HotRegion
{
public:
	FolderIdleView* m_pView;
	FICriteria m_crit;
	FICention m_cent;

	SorterButton(FolderIdleView* pView, FICriteria crit, FICention cent)
		: HotRegion(pView->m_header.ptr())
		, m_pView(pView)
		, m_crit(crit)
		, m_cent(cent)
	{}

	virtual Result msg_paint()
	{
		PaintContext pcontext(this);
		OffscreenContext context(&pcontext);

		context.setFont(Font::eSmall);
		context.rect(Brush::eActiveBorder, 0, 0, context.getWidth(), context.getHeight());

		int iPriority = m_pView->getSortButton(m_crit, m_cent);

		Brush brush = (iPriority == 0 ? Brush::eAppWorkspace : Brush::eWindowText);
		if (m_cent == eAscending)
		{
			context.triangle(brush, 0, context.getHeight(), context.getWidth(), context.getHeight(), context.getWidth() / 2, 0);
		}
		else
		{
			context.triangle(brush, 0, 0, context.getWidth(), 0, context.getWidth() / 2, context.getHeight());
		}

		if (iPriority != 0)
		{
			context.setTextAlignCenter();
			context.setTextColor(Color::eWindow);
			context.setTransparentMode();
			context.text(UTF16::format(L"%d", iPriority), context.getWidth() / 2, 0);
		}

		context.blit();
		return eOk;
	}

	virtual void onClick()
	{
		m_pView->clickSortButton(m_crit, m_cent);
	}
};

FolderIdleView::FolderIdleView(GUI* pParent)
	: GUI(pParent, WS_CHILD)
{
	Memory::zero(&m_scroll, sizeof(m_scroll));
	m_scroll.cbSize = sizeof(m_scroll);
	m_scroll.fMask = SIF_ALL;

	m_header = NewObject(FolderListHeaderView, this);
	m_body = NewObject(FolderListBodyView, this);

	m_fullpath_asc = NewObject(SorterButton, this, eFullpath, eAscending);
	m_fullpath_dsc = NewObject(SorterButton, this, eFullpath, eDescending);
	m_basename_asc = NewObject(SorterButton, this, eBasename, eAscending);
	m_basename_dsc = NewObject(SorterButton, this, eBasename, eDescending);
	m_size_asc = NewObject(SorterButton, this, eSize, eAscending);
	m_size_dsc = NewObject(SorterButton, this, eSize, eDescending);
	m_type_asc = NewObject(SorterButton, this, eType, eAscending);
	m_type_dsc = NewObject(SorterButton, this, eType, eDescending);
	m_link_asc = NewObject(SorterButton, this, eLink, eAscending);
	m_link_dsc = NewObject(SorterButton, this, eLink, eDescending);
	m_content_asc = NewObject(SorterButton, this, eContent, eAscending);
	m_content_dsc = NewObject(SorterButton, this, eContent, eDescending);
	m_created_asc = NewObject(SorterButton, this, eCreated, eAscending);
	m_created_dsc = NewObject(SorterButton, this, eCreated, eDescending);
	m_modified_asc = NewObject(SorterButton, this, eModified, eAscending);
	m_modified_dsc = NewObject(SorterButton, this, eModified, eDescending);
	m_marked_asc = NewObject(SorterButton, this, eMarked, eAscending);
	m_marked_dsc = NewObject(SorterButton, this, eMarked, eDescending);

	calculateMeasurements();

}

void FolderIdleView::loadScroll()
{
	int iRes = GetScrollInfo(m_body->m_hwnd, SB_VERT, &m_scroll);

	Check(iRes != 0);
}

void FolderIdleView::saveScroll()
{
	SetScrollInfo(m_body->m_hwnd, SB_VERT, &m_scroll, true);
	loadScroll();
	m_header->invalidate();
	m_body->invalidate();
}

void FolderIdleView::layoutSorters(int iLeft, int iFTop, int iWidth, int iFHeight, HSorterButton& asc, HSorterButton& dsc)
{
	const int iSorterDim = m_iUnit /2;

	int iHMiddle = iLeft + iWidth / 2;

	int iVMiddle = iFTop + iFHeight / 2;

	int iHalfDim = iSorterDim / 2;

	int iTop = iVMiddle - iHalfDim;
	int iHeight = iSorterDim;
	int iAscLeft = iHMiddle - iSorterDim - iHalfDim;
	int iAscWidth = iSorterDim;
	int iDscLeft = iHMiddle + iHalfDim;
	int iDscWidth = iSorterDim;

	asc->move(iAscLeft, iTop, iAscWidth, iHeight, true);
	dsc->move(iDscLeft, iTop, iDscWidth, iHeight, true);
}

Result FolderIdleView::msg_size(int iWidth, int iHeight)
{
	calculateMeasurements();
	
	m_header->move(0,0,iWidth,m_iUnit*2, true);
	m_body->move(0,m_iUnit*2,iWidth,iHeight-m_iUnit*2, true);
	
	calculateMeasurements();
	
	int iTop = m_iUnit;
	int iHCursor = m_iHalfUnit;

	layoutSorters(iHCursor, iTop, m_iCreatedLen, m_iUnit, m_created_asc, m_created_dsc);

	iHCursor += m_iCreatedLen + m_iHalfUnit;

	layoutSorters(iHCursor, iTop, m_iModifiedLen, m_iUnit, m_modified_asc, m_modified_dsc);

	iHCursor += m_iModifiedLen + m_iHalfUnit;

	layoutSorters(iHCursor, iTop, m_iSizeLen, m_iUnit, m_size_asc, m_size_dsc);

	iHCursor += m_iSizeLen + m_iHalfUnit;

	layoutSorters(iHCursor, iTop, m_iExtLen, m_iUnit, m_type_asc, m_type_dsc);

	iHCursor += m_iExtLen + m_iHalfUnit;

	layoutSorters(iHCursor, iTop, m_iLinkLen, m_iUnit, m_link_asc, m_link_dsc);

	iHCursor += m_iLinkLen + m_iHalfUnit;

	layoutSorters(iHCursor, iTop, m_iContentLen, m_iUnit, m_content_asc, m_content_dsc);

	iHCursor += m_iContentLen + m_iHalfUnit;

	layoutSorters(iHCursor, iTop, m_iUnit, m_iUnit, m_marked_asc, m_marked_dsc);

	iHCursor += m_iUnit + m_iHalfUnit;

	layoutSorters(iHCursor + m_iUnit, iTop, m_iUnit, m_iUnit, m_fullpath_asc, m_fullpath_dsc);
	layoutSorters(iHCursor + 3*m_iUnit, iTop, m_iUnit, m_iUnit, m_basename_asc, m_basename_dsc);

	return eDefault;
};

void FolderIdleView::attachImage(const HFolderImage& image)
{
	m_image = image;
	calculateMeasurements();
}

Result FolderIdleView::msg_paint()
{
	return eDefault;
}

void FolderIdleView::drawHeader(OffscreenContext& context)
{
	context.setFont(Font::eStandard);
	context.setTransparentMode();

	context.rect(Brush::eActiveBorder, 0, 0, context.getWidth(), context.getHeight());

	int iTop = 0;
	int iHCursor = m_iHalfUnit;

	context.setTextAlignCenter();
	context.text(L"Created", iHCursor + m_iCreatedLen / 2, iTop);
	iHCursor += m_iCreatedLen + m_iHalfUnit;

	context.text(L"Modified", iHCursor + m_iModifiedLen / 2, iTop);
	iHCursor += m_iModifiedLen + m_iHalfUnit;

	context.text(L"Size", iHCursor + m_iSizeLen / 2, iTop);
	iHCursor += m_iSizeLen + m_iHalfUnit;

	context.text(L"Type", iHCursor + m_iExtLen / 2, iTop);
	iHCursor += m_iExtLen + m_iHalfUnit;

	context.text(L"Link", iHCursor + m_iLinkLen / 2, iTop);
	iHCursor += m_iLinkLen + m_iHalfUnit;

	context.text(L"Content", iHCursor + m_iContentLen / 2, iTop);
	iHCursor += m_iContentLen + m_iHalfUnit;

	context.rect(Brush::eWindowText, iHCursor + 2, iTop + 2, m_iUnit - 4, m_iUnit - 4);

	iHCursor += m_iUnit + m_iHalfUnit;

	context.setTextAlignLeft();
	context.text(L"Path\\Filename", iHCursor, iTop);
}

void FolderIdleView::drawBody(OffscreenContext& context)
{
	context.setFont(Font::eStandard);
	context.setTransparentMode();

	context.rect(Brush::eWindow, 0, 0, context.getWidth(), context.getHeight());

	if (m_image.isSet())
	{
		loadScroll();
		int iFirstLine = m_scroll.nPos;
		int iLastLine = iFirstLine + m_scroll.nPage + 1;

		if (iLastLine > m_image->getNoOfItems())
			iLastLine = m_image->getNoOfItems();

		int iNumLines = iLastLine - iFirstLine;

		for (int iLine = 0; iLine < iNumLines; iLine++)
		{
			FIItem* item = m_image->getItem(iFirstLine + iLine);

			int iTop = iLine * m_iUnit;

			int iHCursor = m_iHalfUnit;

			context.text(item->getCreatedShortString(), iHCursor, iTop);
			iHCursor += m_iCreatedLen + m_iHalfUnit;

			context.text(item->getModifiedShortString(), iHCursor, iTop);
			iHCursor += m_iModifiedLen + m_iHalfUnit;

			context.setTextAlignRight();
			context.text(item->getSizeShortString(), iHCursor + m_iSizeLen, iTop);
			context.setTextAlignLeft();
			iHCursor += m_iSizeLen + m_iHalfUnit;

			context.setTextAlignCenter();
			context.text(item->getExtShortString(), iHCursor + m_iExtLen / 2, iTop);
			context.setTextAlignLeft();
			iHCursor += m_iExtLen + m_iHalfUnit;

			UTF16 sLinkShortString = item->getLinkShortString();
			UTF16 sContentShortString = item->getContentShortString();

			if (sLinkShortString == sContentShortString)
			{
				int iDoubleLen = m_iLinkLen + m_iHalfUnit + m_iContentLen;

				context.setTextAlignCenter();
				context.text(sLinkShortString, iHCursor + iDoubleLen / 2, iTop);
				context.setTextAlignLeft();
				iHCursor += iDoubleLen + m_iHalfUnit;
			}
			else
			{
				context.setTextAlignCenter();
				context.text(sLinkShortString, iHCursor + m_iLinkLen / 2, iTop);
				context.setTextAlignLeft();
				iHCursor += m_iLinkLen + m_iHalfUnit;

				context.setTextAlignCenter();
				context.text(sContentShortString, iHCursor + m_iContentLen / 2, iTop);
				context.setTextAlignLeft();
				iHCursor += m_iContentLen + m_iHalfUnit;
			}

			context.rect(Brush::eWindowText, iHCursor + 2, iTop + 2, m_iUnit - 4, m_iUnit - 4);
			context.rect(Brush::eWindow, iHCursor + 3, iTop + 3, m_iUnit - 6, m_iUnit - 6);
			Real64 f = item->getMarkedState();
			if (f == 1)
				context.rect(Brush::eWindowText, iHCursor + 4, iTop + 4, m_iUnit - 8, m_iUnit - 8);
			else if (f > 0)
			{
				int iH = (m_iUnit - 8) / 2;
				context.rect(Brush::eWindowText, iHCursor + 4, iTop + 4 + iH, m_iUnit - 8, m_iUnit - 8 - iH);
			}

			iHCursor += m_iUnit + m_iHalfUnit;

			context.text(item->getPath(), iHCursor, iTop);
		}
	};
};

void FolderIdleView::calculateMeasurements()
{
	DrawingContext context(this);

	Font std = Font::eStandard;

	context.setFont(std);
	m_iUnit = getTextHeight(std, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	m_iHalfUnit = m_iUnit / 2 + 3;
	m_iUnit = 2 * m_iHalfUnit;

	int iWWWWWidth = getTextWidth(std, L"WWWW");
	int iWWWddWidth = getTextWidth(std, L"WWW..");

	if (iWWWWWidth > iWWWddWidth)
		m_iExtLen = iWWWWWidth;
	else
		m_iExtLen = iWWWddWidth;
	
	m_iLinkLen = m_iExtLen;
	m_iContentLen = m_iExtLen;

	m_iSizeLen = getTextWidth(std, L"4.98 W");
	m_iCreatedLen = getTextWidth(std, L"2099-99-99");
	m_iModifiedLen = m_iCreatedLen;

	if (m_image.isNull())
		return;

	int iNumItems = m_image->getNoOfItems();
	int iContextHeight = context.getHeight() - 2 * m_iUnit;

	loadScroll();
	m_scroll.nMin = 0;
	m_scroll.nPage = iContextHeight / m_iUnit + 1;
	m_scroll.nMax = iNumItems;

	if (m_scroll.nMax < 0)
		m_scroll.nMax = 0;

	saveScroll();
}

Result FolderIdleView::body_vscroll_bottom()
{
	loadScroll();
	m_scroll.nPos = m_scroll.nMax;
	saveScroll();
	return eOk;
}

Result FolderIdleView::body_vscroll_endscroll()
{
	return eOk;
}

Result FolderIdleView::body_vscroll_linedown()
{
	loadScroll();
	m_scroll.nPos++;
	if (m_scroll.nPos > m_scroll.nMax)
		m_scroll.nPos = m_scroll.nMax;
	saveScroll();
	return eOk;
}

Result FolderIdleView::body_vscroll_lineup()
{
	loadScroll();
	if (m_scroll.nPos > 0)
		m_scroll.nPos--;
	saveScroll();
	return eOk;
}

Result FolderIdleView::body_vscroll_pagedown()
{
	loadScroll();
	m_scroll.nPos += m_scroll.nPage;
	if (m_scroll.nPos > m_scroll.nMax)
		m_scroll.nPos = m_scroll.nMax;
	saveScroll();
	return eOk;
}

Result FolderIdleView::body_vscroll_pageup()
{
	loadScroll();
	m_scroll.nPos -= m_scroll.nPage;
	if (m_scroll.nPos < 0)
		m_scroll.nPos = 0;
	saveScroll();
	return eOk;
}

Result FolderIdleView::body_vscroll_thumbposition(UInt16 iPos)
{
	loadScroll();
	m_scroll.nPos = m_scroll.nTrackPos;
	saveScroll();
	return eOk;
}

Result FolderIdleView::body_vscroll_thumbtrack(UInt16 iPos)
{
	loadScroll();
	m_scroll.nPos = m_scroll.nTrackPos;
	saveScroll();
	return eOk;
}

Result FolderIdleView::body_vscroll_top()
{
	loadScroll();
	m_scroll.nPos = 0;
	saveScroll();
	return eOk;
}

void FolderIdleView::clickSortButton(FICriteria crit, FICention cent)
{
	if (m_image.isNull())
	{
		Check();
		return;
	}

	if (getSortButton(crit, cent) == 1)
		m_image->sort(crit, eNone);
	else
		m_image->sort(crit, cent);
}
	
int FolderIdleView::getSortButton(FICriteria crit, FICention buttonCent)
{
	if (m_image.isNull())
	{
		Check();
		return 0;
	}

	int iPriority = 0;
	FICention activeCent = eNone;
	m_image->sortStatus(crit, iPriority, activeCent);

	if (buttonCent == activeCent)
		return iPriority;
	else
		return 0;
}


void FolderIdleView::showHoverTipFor(int iX, int iY)
{

}

void FolderIdleView::hideHoverTip()
{

}

void FolderIdleView::click(int iDownX, int iDownY, int iUpX, int iUpY)
{
	int iHCursor = m_iHalfUnit;
	iHCursor += m_iCreatedLen + m_iHalfUnit;
	iHCursor += m_iModifiedLen + m_iHalfUnit;
	iHCursor += m_iSizeLen + m_iHalfUnit;
	iHCursor += m_iExtLen + m_iHalfUnit;
	iHCursor += m_iLinkLen + m_iHalfUnit;
	iHCursor += m_iContentLen + m_iHalfUnit;

	int iLeftBox = iHCursor;
	int iRightBox = iHCursor + m_iUnit;

	int iVOffset = 2 * m_iUnit;

	if (!(iDownX >= iLeftBox && iUpX >= iLeftBox && iDownX < iRightBox && iUpX < iRightBox))
		return;

	int iDownIndex = (iDownY) / m_iUnit;
	int iUpIndex = (iUpY) / m_iUnit;

	if (iDownIndex != iUpIndex)
		return;

	int iIndex = iUpIndex;

	clickItem(iIndex);
}

void FolderIdleView::clickItem(int iIndex)
{
	loadScroll();

	int iFirstLine = m_scroll.nPos;

	if (m_image.isNull())
		return;

	int iItemIndex = iFirstLine + iIndex;
	
	if (iItemIndex >= m_image->getNoOfItems())
		return;

	m_image->toggleMarkItem(iItemIndex);
}