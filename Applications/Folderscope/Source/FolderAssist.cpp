// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderAssist.h"

#include "PaintContext.h"
#include "FontProvider.h"
#include "OffscreenContext.h"

#include "WelcomeView.h"

FolderAssist::FolderAssist(WelcomeView* pParent, int iUnit)
	: GUI(pParent, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_BORDER)
	, m_iNumLines(0)
	, m_iUnit(iUnit)
	, m_iSelected(-1)
	, m_pWelcomeView(pParent)
{
	m_sugprov = NewObject(FolderSuggestionProvider);
	m_sugprov->addListener(this);
	m_sugprov->start();

	m_iLineHeight = getTextHeight(Font::eStandard, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
}

void FolderAssist::onDownArrow()
{
	Synchronizer sync(m_lock);

	int iMaxSuggestion = m_suggestions.size();

	if (m_iSelected < (iMaxSuggestion - 1))
	{
		m_iSelected++;
		m_pWelcomeView->pathSelected(m_suggestions[m_iSelected].path);
		invalidate();
	}
}

void FolderAssist::onUpArrow()
{
	Synchronizer sync(m_lock);

	if (m_iSelected > 0)
	{
		m_iSelected--;
		m_pWelcomeView->pathSelected(m_suggestions[m_iSelected].path);
		invalidate();
	}
}

Result FolderAssist::msg_size(int iWidth, int iHeight)
{
	Synchronizer sync(m_lock);

	m_iNumLines = iHeight / m_iLineHeight;

	m_suggestions.resize(m_iNumLines);

	if (m_iSelected > m_iNumLines - 1)
		m_iSelected = m_iNumLines - 1;
	dirty();

	return eOk;
}

Result FolderAssist::msg_paint()
{
	PaintContext pcontext(this);
	OffscreenContext context(&pcontext); 

	context.setFont(Font::eStandard);
	context.rect(Brush::eWindow, 0,0,context.getWidth(),context.getHeight());

	int i = 0;
	while (true)
	{
		UTF16 sNextPath, sNextNotes;

		{
			Synchronizer sync(m_lock);
			if (i >= m_suggestions.size())
				break;

			sNextPath = m_suggestions[i].path;
			sNextNotes = m_suggestions[i].notes;
		}

		bool bSelected = (i == m_iSelected);

		int iPathWidth, iPathHeight;
		context.calcTextSize(sNextPath, iPathWidth, iPathHeight);


		if (bSelected)
		{
			context.rect(Brush::eHighlight, 0, i*m_iLineHeight, context.getWidth(), m_iUnit - 3);
			context.setTextColor(Color::eHighlightText);
			context.text(sNextPath, 1, i*m_iLineHeight);
			context.setTextColor(Color::eGrayText);
			context.text(sNextNotes, 1 + iPathWidth + m_iUnit / 2, i*m_iLineHeight);
		}
		else
		{
			context.setTextColor(Color::eWindowText);
			context.text(sNextPath, 1, i*m_iLineHeight);
			context.setTextColor(Color::eGrayText);
			context.text(sNextNotes, 1 + iPathWidth + m_iUnit / 2, i*m_iLineHeight);
		}

		i++;
	}

	context.blit();

	return eOk;
}

void FolderAssist::setPrefix(const UTF16& sPrefix)
{
	Synchronizer sync(m_lock);

	if (m_sPrefix != sPrefix)
	{
		m_sPrefix = sPrefix;
		dirty();
	}
}

void FolderAssist::async_GetSuggestions()
{
	m_sugprov->search(m_sPrefix, m_suggestions.size());
}

void FolderAssist::notifySuggestions(HFolderSuggestionList hList)
{
	m_iSelected = -1;
	FolderSuggestionList::Iterator it(*hList);
	FolderSuggestion sug;

	int i = 0;

	while (it(sug))
	{
		Synchronizer sync(m_lock);

		if (i >= m_suggestions.size())
			break;
		
		m_suggestions[i] = sug;
		i++;
	}

	FolderSuggestion emptysug;
	for (int j = i; true; j++)
	{
		Synchronizer sync(m_lock);

		if (j >= m_suggestions.size())
			break;
		
		m_suggestions[j] = emptysug;
	}

	invalidate();
}

void FolderAssist::dirty()
{
	Synchronizer sync(m_lock);

	m_bDirty = true;

	async_GetSuggestions();
}

FolderAssist::~FolderAssist()
{
	m_sugprov->shutdown();
}

void FolderAssist::onEnterKey()
{
	Synchronizer sync(m_lock);

	if (m_iSelected == -1)
	{
		m_pWelcomeView->clickBrowseButton();
	}
	else
	{
		m_pWelcomeView->promoteCurrentSelection();
	}
}
