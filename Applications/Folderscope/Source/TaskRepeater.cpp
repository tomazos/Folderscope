// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "TaskRepeater.h"

#include "TaskTracker.h"

HTaskRepeaterManager
TaskRepeaterManager::instance()
{
	GlobalSync;

	static HTaskRepeaterManager p;
	if (!p.isSet())
	{
		p = NewObject(TaskRepeaterManager);
		p->start();
	}

	return p;
}

TaskRepeaterManager::~TaskRepeaterManager()
{
	shutdown();
}

void TaskRepeaterManager::run()
{
	while (true)
	{
		m_continue.pass();

		if (m_shutdown.occured())
			return;

		TaskTracker::instance()->add(buildMethodTask(this, &TaskRepeaterManager::executeRepeaters), TaskTracker::eMainThread);
		sleep(100);
	}
}

void TaskRepeaterManager::executeRepeaters()
{
	Synchronizer sync(m_lock);

	if (m_repeaters.empty())
		return;

	TreeSet<TaskRepeater*>::Iterator it(m_repeaters);
	TaskRepeater* pRepeater;
	while (it(pRepeater))
		pRepeater->execute();

	m_continue.open();
}

void TaskRepeaterManager::shutdown()
{
	m_shutdown.cause();
	m_continue.open();
	join();
}

void TaskRepeaterManager::add(TaskRepeater* pRepeater)
{
	Synchronizer sync(m_lock);

	m_repeaters += pRepeater;
	m_continue.open();
}

void TaskRepeaterManager::del(TaskRepeater* pRepeater)
{
	Synchronizer sync(m_lock);
	m_repeaters -= pRepeater;
}

TaskRepeater::~TaskRepeater()
{
	stop();
}

void TaskRepeater::start()
{
	TaskRepeaterManager::instance()->add(this);
}

void TaskRepeater::stop()
{
	TaskRepeaterManager::instance()->del(this);
}

void TaskRepeater::execute()
{
	if (m_task.isSet())
		m_task->execute();
}
