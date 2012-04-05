// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "TaskTracker.h"

#include "Application.h"

void TaskTracker::add(HTask task, EExecContext context)
{
	switch (context)
	{
	case eMainThread: addMainThread(task); break;
	case eOwnThread: addOwnThread(task); break;
	}
}

class TaskExecutorThread : public Thread
{
public:
	HTask m_task;

	TaskExecutorThread(HTask task) : m_task(task) {}
	
	virtual void run()
	{
		m_task->execute();
	}
};

void TaskTracker::addOwnThread(HTask task)
{
	HThread thread = NewObject(TaskExecutorThread, task);
	thread->start();
}

void TaskTracker::addMainThread(HTask task)
{
	Synchronizer lock(m_lock);

	if (Application::instance().isNull())
	{
		logerr("TaskTracker: Tasks requested before application started");
	}
	else
	{
		s_tasks.add(task);

		if (Application::instance().isSet())
			Application::instance()->tasksAvailable();
	}
}

void TaskTracker::processTasks_MainThread()
{
	Synchronizer lock(m_lock);

	HTask task;
	while (s_tasks.remove(task, 0))
	{
		if (task.isSet())
			task->execute();
	}
}

TaskTracker* TaskTracker::instance()
{
	GlobalSync;

	static TaskTracker* p(NULL);
	if (p == NULL)
		p = NewObject(TaskTracker);
	return p;
}
