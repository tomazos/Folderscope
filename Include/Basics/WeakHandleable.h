// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Lock.h"

#include "Basics/TreeSet.h"

class WeakHandleable
{
public:
	WeakHandleable();
	virtual ~WeakHandleable();

	bool isHandled();

	void addHandle();
	void removeHandle();

	class Listener
	{
	public:
		virtual void notifyDeleted() = 0;
	};

	void addDeletionListener(Listener*);
	void removeDeletionListener(Listener*);

private:
	
	void notifyDeleted();

	TreeSet<Listener*> m_listeners;

	Lock m_lock;
	Size m_iNumHandles;
};
