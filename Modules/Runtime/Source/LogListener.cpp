// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/LogListener.h"

#include "Runtime/Log.h"

LogListener::LogListener()
{
	Log::registerListener(this);
}

LogListener::~LogListener()
{
	Log::unregisterListener(this);
}
