// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Runtime/LogEntry.h"

DeclHandleable(LogListener);
class LogListener : public Handleable
{
public:
	LogListener();
	virtual ~LogListener();

	virtual void processLogEntry(HLogEntry) = 0;
};
