// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Handleable.h"

DeclHandleable(Task);
class Task : public Handleable
{
public:
	virtual void execute() = 0;
};
