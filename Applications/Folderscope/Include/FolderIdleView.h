// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "GUI.h"
#include "FolderImage.h"
#include "OffscreenContext.h"

class SorterButton;

DeclWeakHandleable(FolderListHeaderView);
DeclWeakHandleable(FolderListBodyView);
DeclWeakHandleable(SorterButton);

DeclWeakHandleable(FolderIdleView);
class FolderIdleView : public GUI
{
public:
	FolderIdleView(GUI* pParent);
	
	void attachImage(const HFolderImage& image);

	virtual Result msg_paint();
	virtual Result msg_size(int iWidth, int iHeight);

private:
	SCROLLINFO m_scroll;
	void loadScroll();
	void saveScroll();

	void clickSortButton(FICriteria crit, FICention cent);
	int getSortButton(FICriteria crit, FICention buttonCent);
	void clickItem(int iIndex);

	HFolderImage m_image;
	Lock m_lock;

	void calculateMeasurements();
	int m_iHalfUnit;
	int m_iUnit;

	int m_iCreatedLen;
	int m_iModifiedLen;
	int m_iSizeLen;
	int m_iExtLen;
	int m_iLinkLen;
	int m_iContentLen;

	friend class SorterButton;
	friend class FolderListBodyView;
	friend class FolderListHeaderView;

	void drawBody(OffscreenContext& context);
	void drawHeader(OffscreenContext& context);

	void showHoverTipFor(int iX, int iY);
	void hideHoverTip();
	void click(int iDownX, int iDownY, int iUpX, int iUpY);

	virtual Result body_vscroll_bottom();
	virtual Result body_vscroll_endscroll();
	virtual Result body_vscroll_linedown();
	virtual Result body_vscroll_lineup();
	virtual Result body_vscroll_pagedown();
	virtual Result body_vscroll_pageup();
	virtual Result body_vscroll_thumbposition(UInt16 iPos);
	virtual Result body_vscroll_thumbtrack(UInt16 iPos);
	virtual Result body_vscroll_top();

	HFolderListHeaderView m_header;
	HFolderListBodyView m_body;
	HSorterButton m_fullpath_asc;
	HSorterButton m_fullpath_dsc;
	HSorterButton m_basename_asc;
	HSorterButton m_basename_dsc;
	HSorterButton m_size_asc;
	HSorterButton m_size_dsc;
	HSorterButton m_type_asc;
	HSorterButton m_type_dsc;
	HSorterButton m_link_asc;
	HSorterButton m_link_dsc;
	HSorterButton m_content_asc;
	HSorterButton m_content_dsc;
	HSorterButton m_created_asc;
	HSorterButton m_created_dsc;
	HSorterButton m_modified_asc;
	HSorterButton m_modified_dsc;
	HSorterButton m_marked_asc;
	HSorterButton m_marked_dsc;

	void layoutSorters(int iLeft, int iFTop, int iWidth, int iFHeight, HSorterButton& asc, HSorterButton& dsc);

};
