// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Runtime/LogListener.h"
#include "Runtime/LogEntry.h"

class LogProcessor
{
public:
	LogProcessor();

	void registerListener(LogListener*);
	void unregisterListener(LogListener*);
	void process(HLogEntry);
private:
	Lock m_lock;

	TreeSet<LogListener*> m_listeners;
};
