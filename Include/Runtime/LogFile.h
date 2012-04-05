// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

#include "Runtime/Log.h"

class LogFile : public LogListener
{
public:
	LogFile(Error&, const UTF16& sLogFilePath);
	virtual ~LogFile();

	virtual void processLogEntry(HLogEntry);

private:
	Error m_error;
	HOutputStream m_logFile;
	UTF16 m_sLogFilePath;
};
