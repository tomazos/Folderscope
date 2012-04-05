// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

DeclHandleable(LogEntry);
class LogEntry : public Handleable
{
public:
	enum ELogLevel
	{
		eInfo,
		eWarning,
		eError
	};

	LogEntry(ELogLevel eLevel, const UTF16& sMessage);
	virtual ~LogEntry() {}

	ELogLevel getLevel();
	UTF16 getMessage();
	UTF16 getFullMessage();
	TimePoint getWhen();

private:
	ELogLevel m_eLevel;
	UTF16 m_sMessage;
	TimePoint m_when;
};
