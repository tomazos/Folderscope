// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Runtime.h"

#include "GUI.h"
#include "LogEntryQueue.h"

class LogView : public GUI, public LogEntryQueue::Listener
{
public:
	LogView(GUI* pParent);
	virtual ~LogView();

	void processLogEntries();

	virtual void requestProcessing();

private:
	void append(UTF16 sLogLine);

	LogEntryQueue m_logEntryQueue;
};
