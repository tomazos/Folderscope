// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

class Path;

class PathWalker
{
public:
	virtual void startDir(const Path&, bool& bStop) {};
	virtual void processFile(const Path&, bool& bStop) {};
	virtual void endDir(const Path&, bool& bStop) {};

	virtual void onError(const Path&, const Error& error, bool& bStop)
	{ Check(); bStop = true; }
};
