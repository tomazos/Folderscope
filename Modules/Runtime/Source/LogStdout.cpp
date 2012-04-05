// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/LogStdout.h"

#include "stdio.h"

#include "Runtime/Log.h"

LogStdout::LogStdout()
{
	Log::infof(L"Attaching stdout log");
}

LogStdout::~LogStdout()
{
	Log::infof(L"Detaching stdout log");
}

void LogStdout::processLogEntry(HLogEntry hEntry)
{
	if (hEntry.isSet())
	{
		UTF16 sLogLine;
	
		switch (hEntry->getLevel())
		{
		case LogEntry::eInfo:
			sLogLine = UTF16(hEntry->getWhen().toString())
				+ L": " + hEntry->getMessage() + L"\r\n";
			break;
		case LogEntry::eWarning:
			sLogLine = UTF16(hEntry->getWhen().toString())
				+ L": WARNING: " + hEntry->getMessage() + L"\r\n";
			break;
		case LogEntry::eError:
			sLogLine = UTF16(hEntry->getWhen().toString())
				+ L": ERROR: " + hEntry->getMessage() + L"\r\n";
			break;
		}
		
		wprintf(L"%s", sLogLine.ptr());
	}
}
