// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/Log.h"

void
Log::registerListener(LogListener* pListener)
{
	processor()->registerListener(pListener);
};

void
Log::unregisterListener(LogListener* pListener)
{
	processor()->unregisterListener(pListener);
}

void
Log::infof(const Chr16* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);
	vinfof(sFormat, args);
	va_end(args);
};

void
Log::vinfof(const Chr16* sFormat, va_list arg)
{
	UTF16 sLogLine = UTF16::vformat(sFormat, arg);
	processor()->process(NewObject(LogEntry, LogEntry::eInfo, sLogLine));
};

void
Log::warnf(const Chr16* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);
	vwarnf(sFormat, args);
	va_end(args);
}

void
Log::vwarnf(const Chr16* sFormat, va_list arg)
{	
	UTF16 sLogLine = UTF16::vformat(sFormat, arg);
	processor()->process(NewObject(LogEntry, LogEntry::eWarning, sLogLine));
}

void
Log::errorf(const Chr16* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);
	verrorf(sFormat, args);
	va_end(args);
}

void
Log::verrorf(const Chr16* sFormat, va_list arg)
{
	UTF16 sLogLine = UTF16::vformat(sFormat, arg);
	processor()->process(NewObject(LogEntry, LogEntry::eWarning, sLogLine));
}

LogProcessor* Log::processor()
{
	GlobalSync;

	static LogProcessor* p(NULL);
	if (!p)
		p = NewObject(LogProcessor);
	return p;
}
