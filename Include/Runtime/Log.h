// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Runtime/LogListener.h"
#include "Runtime/LogProcessor.h"

#define logerr(...) Log::errorf(L##__VA_ARGS__);
#define logwarn(...) Log::warnf(L##__VA_ARGS__);
#define loginfo(...) Log::infof(L##__VA_ARGS__);


class Log
{
public:
	static void registerListener(LogListener* pListener);
	static void unregisterListener(LogListener* pListener);

	static void infof(const Chr16* sFormat, ...);
	static void warnf(const Chr16* sFormat, ...);
	static void errorf(const Chr16* sFormat, ...);

	static void vinfof(const Chr16* sFormat, va_list arg);
	static void vwarnf(const Chr16* sFormat, va_list arg);
	static void verrorf(const Chr16* sFormat, va_list arg);
private:
	static LogProcessor* processor();
};
