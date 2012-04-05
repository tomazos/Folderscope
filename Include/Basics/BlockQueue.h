// (C) 2009 Tomazos-Jager Software Labs
#pragma once
#include "global.h"

#include "Basics/Memory.h"
#include "Basics/Check.h"
#include "Basics/Lock.h"
#include "Basics/Turnstile.h"
#include "Basics/Event.h"
#include "Basics/Synchronizer.h"

template<class T>
class BlockQueue
{
public:
	BlockQueue();
	~BlockQueue();

	bool empty();
	int size();

	void add(T);
	bool remove(T&);
	bool remove(T&, int iTimeoutMilliseconds);

	void shutdown();

private:

	class Item
	{
	public:
		T m_item;
		Item* m_next;
	};

	Item* m_head;
	Item* m_tail;

	Lock m_lock;
	Turnstile m_turnstile;
	Event m_shutdown;
};

template<class T>
BlockQueue<T>::BlockQueue()
	: m_head(NULL)
	, m_tail(NULL)
{

}

template<class T>
BlockQueue<T>::~BlockQueue()
{
	Check(m_head == NULL);

	while (m_head != NULL)
	{
		Item* p = m_head;
		m_head = p->m_next;
		DelObject(p);
		p = NULL;
	}
}

template<class T>
bool BlockQueue<T>::empty()
{
	Synchronizer sync(m_lock);

	return m_head == NULL;
}

template<class T>
int BlockQueue<T>::size()
{
	Synchronizer sync(m_lock);

	int iCount = 0;

	for (Item* p = m_head; p != NULL; p = p->m_next)
		iCount++;

	return iCount;
}

template<class T>
void BlockQueue<T>::add(T tNewItem)
{
	Synchronizer sync(m_lock);

	Item* pNewItem = NewObject(Item);
	pNewItem->m_item = tNewItem;
	pNewItem->m_next = NULL;

	if (m_tail != NULL)
		m_tail->m_next = pNewItem;

	m_tail = pNewItem;

	if (m_head == NULL)
		m_head = pNewItem;

	m_turnstile.open();
}

template<class T>
bool BlockQueue<T>::remove(T& t)
{
	return remove(t, INFINITE);
};

template<class T>
bool BlockQueue<T>::remove(T& t, int iTimeoutMilliseconds)
{
	if (m_shutdown.occured())
		return false;

	WindowsHandle h[2];
	h[0] = m_shutdown.getHandle();
	h[1] = m_turnstile.getHandle();
	int iResult = WaitForMultipleObjects(2, h, false, iTimeoutMilliseconds);
	switch(iResult)
	{
	case WAIT_OBJECT_0: return false;
	case WAIT_OBJECT_0+1: break;
	case WAIT_TIMEOUT: return false;
	default: Check();	return false;
	}
	
	{
		Synchronizer sync(m_lock);
		if (m_head == NULL)
		{
			Check();
			return false;
		}

		Item* pItem = m_head;

		if (m_tail == pItem)
			m_tail = NULL;

		m_head = pItem->m_next;

		if (m_head != NULL)
			m_turnstile.open();
		
		t = pItem->m_item;
		DelObject(pItem);
		pItem = NULL;
		return true;
	}
}

template<class T>
void BlockQueue<T>::shutdown()
{
	m_shutdown.cause();
}