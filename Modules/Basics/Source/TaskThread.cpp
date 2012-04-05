// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/TaskThread.h"

TaskThread::TaskThread()
{}

TaskThread::~TaskThread()
{
	shutdown();
}

void TaskThread::add(HTask task)
{
	m_tasks.add(task);
}

void TaskThread::run()
{
	HTask task;

	while (m_tasks.remove(task))
		task->execute();
}

void TaskThread::shutdown()
{
	m_tasks.shutdown();
}
