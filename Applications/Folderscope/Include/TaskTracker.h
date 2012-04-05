#pragma once
#include "global.h"

#include "Basics.h"

class TaskTracker
{
public:
	static TaskTracker* instance();

	enum EExecContext
	{
		eMainThread,
		eOwnThread
	};

	void add(HTask task, EExecContext);

	void processTasks_MainThread();

private:
	void addMainThread(HTask);
	void addOwnThread(HTask);

	Lock m_lock;
	BlockQueue<HTask> s_tasks;
};
