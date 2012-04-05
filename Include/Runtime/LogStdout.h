// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

#include "Runtime/LogListener.h"

class LogStdout : public LogListener
{
public:
	LogStdout();
	virtual ~LogStdout();

	virtual void processLogEntry(HLogEntry);
};
