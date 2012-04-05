// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class TaskRepeater;

DeclWeakHandleable(TaskRepeaterManager);
class TaskRepeaterManager : public Thread
{
public:
	static HTaskRepeaterManager instance();

	TaskRepeaterManager::~TaskRepeaterManager();

	virtual void run();

	void executeRepeaters();

	void shutdown();

	void add(TaskRepeater* pRepeater);

	void del(TaskRepeater* pRepeater);

private:
	Lock m_lock;
	Event m_shutdown;
	Turnstile m_continue;

	TreeSet<TaskRepeater*> m_repeaters;
};

class TaskRepeater
{
public:
	void setTask(HTask task)
	{
		m_task = task;
	}

	~TaskRepeater();

	void start();
	void stop();
	void execute();

private:
	HTask m_task;
};
