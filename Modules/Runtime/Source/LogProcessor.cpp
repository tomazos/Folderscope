// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/LogProcessor.h"

LogProcessor::LogProcessor()
	: m_listeners()
{
}

void
LogProcessor::registerListener(LogListener* pListener)
{
	Synchronizer lock(m_lock);

	m_listeners += pListener;
}

void
LogProcessor::unregisterListener(LogListener* pListener)
{
	Synchronizer lock(m_lock);

	m_listeners -= pListener;
}

void
LogProcessor::process(HLogEntry hEntry)
{
	Synchronizer lock(m_lock);

	TreeSet<LogListener*>::Iterator it(m_listeners);
	LogListener* pListener;

	while (it(pListener))
	{
		pListener->processLogEntry(hEntry);
	}
};
