// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/WeakHandleable.h"
#include "Basics/Synchronizer.h"

#define SyncHandle Synchronizer sync(m_lock);

WeakHandleable::WeakHandleable()
	: m_iNumHandles(0)
{
}

WeakHandleable::~WeakHandleable()
{
}

bool WeakHandleable::isHandled()
{
	return m_iNumHandles > 0;
}

void
WeakHandleable::addHandle()
{
	SyncHandle;

	m_iNumHandles++;
}

void
WeakHandleable::removeHandle()
{
	bool bDeleteSelf = false;
	{
		SyncHandle;

		m_iNumHandles--;

		if (m_iNumHandles == 0)
		{
			notifyDeleted();
			if (m_iNumHandles == 0)
				bDeleteSelf = true;
		}
	}

	if (bDeleteSelf)
		DelObject(this);
}

void WeakHandleable::addDeletionListener(Listener* pListener)
{
	SyncHandle;

	m_listeners += pListener;
};

void WeakHandleable::removeDeletionListener(Listener* pListener)
{
	SyncHandle;

	m_listeners -= pListener;
}

void WeakHandleable::notifyDeleted()
{
	SyncHandle;

	TreeSet<Listener*>::Iterator it(m_listeners);
	Listener* pListener;
	while (it(pListener))
	{
		pListener->notifyDeleted();
	}
	m_listeners.clear();
}
