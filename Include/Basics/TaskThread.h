// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Lock.h"
#include "Basics/BlockQueue.h"
#include "Basics/Thread.h"
#include "Basics/Task.h"

DeclWeakHandleable(TaskThread);
class TaskThread : public Thread
{
public:
	TaskThread();
	~TaskThread();

	void add(HTask task);
		
	virtual void run();

	void shutdown();

private:
	BlockQueue<HTask> m_tasks;
};
