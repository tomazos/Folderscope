// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Basics/Handleable.h"

#include "Basics/Memory.h"

#include <typeinfo.h>

void
Handleable::addHandle()
{
	InterlockedIncrement(&m_iNumHandles);
}

void
Handleable::removeHandle()
{
	if (InterlockedDecrement(&m_iNumHandles) == 0)
		DelObject(this);
}
