// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "TabView.h"

#include "CommCtrl.h"
#include "Richedit.h"

#include "FontProvider.h"

#include "Runtime.h"

TabView::TabView(GUI* pParent)
	: GUI(pParent, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, eDefaultCursor, WC_TABCONTROL)
	, m_iCurrentIndex(-1)
{
	setFont(Font::eBold);
};

Size TabView::getNumPanes()
{
	return m_panes.size();	
}

void TabView::setCurrentPane(HGUI pane)
{
	for (int i = 0; i < m_panes.size(); i++)
	{
		if (m_panes[i] == pane)
		{
			setCurrentIndex(i);
			return;
		}
	}
	Check();
}

int TabView::getCurrentIndex()
{
	return m_iCurrentIndex;
}

void TabView::setCurrentIndex(Size iIndex)
{
	if (m_panes.inbounds(iIndex) || iIndex == -1)
	{
		if (iIndex != m_iCurrentIndex)
		{
			if (m_iCurrentIndex != -1)
				hideIndex(m_iCurrentIndex);

			m_iCurrentIndex = iIndex;
			TabCtrl_SetCurSel(m_hwnd, m_iCurrentIndex);

			if (m_iCurrentIndex != -1)
				showIndex(m_iCurrentIndex);
		}
	}
	else
		Check();
}

HGUI TabView::getPane(Size iIndex)
{
	if (m_panes.inbounds(iIndex))
		return m_panes[iIndex];
	else
	{
		Check();
		return NULL;
	}
}

int TabView::getIndex(HGUI pane)
{
	for (int i = 0; i < m_panes.size(); i++)
	{
		if (m_panes[i] == pane)
			return i;
	}
	Check();
	return -1;
}

void TabView::pushPane(UTF16 sTabName, HGUI pane)
{
	insertPane(getNumPanes(), sTabName, pane);
}

void TabView::insertPane(Size iIndex, UTF16 sTabName, HGUI pane)
{
	m_panes.insert(iIndex, pane);
	TCITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = (LPWSTR) sTabName.ptr();
	TabCtrl_InsertItem(m_hwnd, iIndex, &item);
	setPaneRects();
	if (m_panes.size() == 1)
		setCurrentIndex(0);
}

void TabView::delPane(HGUI pane)
{
	delIndex(getIndex(pane));
}

void TabView::delIndex(Size iIndex)
{
	if (!m_panes.inbounds(iIndex))
	{
		Check();
		return;
	}

	if (iIndex == m_iCurrentIndex)
	{
		if (m_iCurrentIndex > 0)
			setCurrentIndex(m_iCurrentIndex - 1);
		else if (m_panes.size() > 1)
			setCurrentIndex(m_iCurrentIndex + 1);
		else
			setCurrentIndex(-1);
	}

	m_panes.del(iIndex);
	TabCtrl_DeleteItem(m_hwnd, iIndex);
}

Result TabView::msg_size(int iWidth, int iHeight)
{
	setPaneRects();

	return eDefault;
};

void TabView::setPaneRects()
{
	RECT r;
	r.top = 0;
	r.left = 0;
	r.right = getWidth();
	r.bottom = getHeight();
	SendMessage(m_hwnd, TCM_ADJUSTRECT, (WPARAM) FALSE, (LPARAM) &r);

	for (int i = 0; i < m_panes.size(); i++)
	{
		m_panes[i]->move(r.left, r.top, r.right - r.left, r.bottom - r.top, true);
	}
}

Result TabView::note_selchange()
{
	setCurrentIndex(TabCtrl_GetCurSel(m_hwnd));
	return eOk;
}

HGUI TabView::getCurrentPane()
{
	int iCurTab = getCurrentIndex();

	if (m_panes.inbounds(iCurTab))
		return m_panes[iCurTab];
	else
	{
		Check();
		return NULL;
	}
}

void TabView::showIndex(int iIndex)
{
	if (m_panes.inbounds(iIndex))
	{
		m_panes[iIndex]->show();
		onPaneShown(m_panes[iIndex]);
		m_panes[iIndex]->invalidate();
	}
	else
		Check();
}

void TabView::hideIndex(int iIndex)
{
	if (m_panes.inbounds(iIndex))
	{
		m_panes[iIndex]->hide();
		onPaneHidden(m_panes[iIndex]);
	}
	else
		Check();
}
