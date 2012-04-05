#include "Basics/ThreadManager.h"

ThreadManager* ThreadManager::instance()
{
	GlobalSync;

	static ThreadManager* p(NULL);
	if (!p)
		p = new ThreadManager;

	return p;
}

void registerThread()
{
	Synchronizer sync(m_lock);

	DWORD threadId = GetCurrentThreadId();

	m_threadIds.add(threadId);

	if (m_bThreadsPaused)

}

void unregisterThread()
{
	Synchronizer sync(m_lock);

	m_threadIds.remove(GetCurrentThreadId());
}
