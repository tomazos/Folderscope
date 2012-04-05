// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "LogView.h"

#include "CommCtrl.h"
#include "Richedit.h"

#include "FontProvider.h"

#include "TaskTracker.h"

void LogView::append(UTF16 sLine)
{
	CHARRANGE oldSelection;
	SendMessage(m_hwnd, EM_EXGETSEL, 0, (LPARAM) &oldSelection);

	CHARRANGE endPoint;
	endPoint.cpMin = -1;
	endPoint.cpMax = -1;
	SendMessage(m_hwnd, EM_EXSETSEL, 0, (LPARAM) &endPoint);

	SETTEXTEX stx;
	stx.flags = ST_SELECTION;
	stx.codepage = 1200;
	SendMessage(m_hwnd, EM_SETTEXTEX, (WPARAM) &stx, (LPARAM) (sLine).ptr());

	SendMessage(m_hwnd, EM_EXSETSEL, 0, (LPARAM) &oldSelection);
	SendMessage(m_hwnd, WM_VSCROLL, (WPARAM) SB_BOTTOM, 0);
}

LogView::LogView(GUI* pParent)
	: GUI(pParent, WS_CHILD | WS_VSCROLL |
        ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		eDefaultCursor, RICHEDIT_CLASS)
{
	setFont(Font::eFixed);

	SendMessage(m_hwnd, EM_SETREADONLY, (WPARAM) TRUE, 0);
	SendMessage(m_hwnd, WM_VSCROLL, (WPARAM) SB_BOTTOM, 0);

	m_logEntryQueue.setListener(this);
}

LogView::~LogView()
{
	m_logEntryQueue.removeListener(this);
}

void LogView::processLogEntries()
{
	UTF16 sLogMessage;

	HLogEntry hEntry;
	while (true)
	{
		hEntry = m_logEntryQueue.takeLogEntry();

		if (hEntry.isSet())
		{
			sLogMessage = sLogMessage + hEntry->getFullMessage() + L"\r\n";
		}
		else
		{
			m_logEntryQueue.notifyEmpty();
			break;
		}
	}

	append(sLogMessage);
};

void LogView::requestProcessing()
{
	HTask task = buildMethodTask(this, &LogView::processLogEntries);
	TaskTracker::instance()->add(task, TaskTracker::eMainThread);
};
