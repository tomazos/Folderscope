// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Handle.h"

#define DeclWeakHandleable(T) class T; typedef Handle<T> H##T; typedef WeakHandle<T> WH##T

#include "Basics/WeakHandleable.h"

template<class T>
class WeakHandle : public WeakHandleable::Listener
{
public:
	WeakHandle();
	WeakHandle(const Handle<T>&);
	WeakHandle(const WeakHandle<T>&);
	~WeakHandle();

	operator Handle<T>() const;

	WeakHandle<T>& operator=(const Handle<T>&);
	WeakHandle<T>& operator=(const WeakHandle<T>&);

	bool isSet() const;
	bool isNull() const;
	void setNull();

private:
	T* m_pTarget;

public:
	virtual void notifyDeleted();
};

template<class T>
WeakHandle<T>::WeakHandle()
	: m_pTarget(NULL)
{

}

template<class T>
WeakHandle<T>::WeakHandle(const WeakHandle<T>& that)
{
	Handle<T> h = that;
	m_pTarget = h.ptr();
	if (h.isSet())
		h->addDeletionListener(this);
}

template<class T>
WeakHandle<T>::WeakHandle(const Handle<T>& that)
{
	m_pTarget = that.ptr();
	if (that.isSet())
		that->addDeletionListener(this);
}

template<class T>
WeakHandle<T>::~WeakHandle()
{
	Handle<T> h = *this;

	if (h.isSet())
		h->removeDeletionListener(this);
}

template<class T>
WeakHandle<T>::operator Handle<T>() const
{
	Handle<T> h = m_pTarget;

	return h;
};

template<class T>
WeakHandle<T>&
WeakHandle<T>::operator=(const Handle<T>& that)
{
	WeakHandle<T> whthat(that);

	return ((*this) = whthat);
}

template<class T>
WeakHandle<T>&
WeakHandle<T>::operator=(const WeakHandle<T>& that)
{
	Handle<T> oldTarget = m_pTarget;

	if (oldTarget.isSet())
		oldTarget->removeDeletionListener(this);

	Handle<T> newTarget = that;

	m_pTarget = newTarget.ptr();

	if (newTarget.isSet())
		newTarget->addDeletionListener(this);

	return (*this);
}


template<class T>
bool
WeakHandle<T>::isSet() const
{
	return (m_pTarget != NULL);
}

template<class T>
bool
WeakHandle<T>::isNull() const
{
	return (m_pTarget == NULL);
}

template<class T>
void
WeakHandle<T>::setNull()
{
	Handle<T> oldTarget = m_pTarget;

	if (oldTarget.isSet())
		h->removeDeletionListener(this);

	m_pTarget = NULL;
}

template<class T>
void WeakHandle<T>::notifyDeleted()
{
	m_pTarget = NULL;
}
