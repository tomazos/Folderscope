// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

template<class T>
class Handle
{
public:
	Handle();
	Handle(T* pTarget);
	Handle(const Handle<T>& that);

	template<class S>
	Handle(const Handle<S>& that);

	template<class S>
	Handle<S> cast();

	template<class S> bool operator == (const Handle<S>&) const;
	template<class S> bool operator != (const Handle<S>& that) const;
	template<class S> bool operator < (const Handle<S>&) const;
	template<class S> bool operator > (const Handle<S>& that) const;
	template<class S> bool operator <= (const Handle<S>&) const;
	template<class S> bool operator >= (const Handle<S>& that) const;

	~Handle();

	T* ptr() const { return m_pTarget; }

	T& operator*() const;
    T* operator->() const;

	Handle<T>& operator=(const Handle<T>&);

	bool isSet() const;
	bool isNull() const;
	void setNull();

private:
	T* m_pTarget;
};

template<class T>
Handle<T>::Handle()
	: m_pTarget(NULL)
{

}

template<class T>
Handle<T>::Handle(T* pTarget)
	: m_pTarget(pTarget)
{
	if (m_pTarget)
		m_pTarget->addHandle();
}

template<class T>
Handle<T>::Handle(const Handle<T>& that)
	: m_pTarget(that.m_pTarget)
{
	if (m_pTarget)
		m_pTarget->addHandle();
}

template<class T>
template<class S>
Handle<T>::Handle(const Handle<S>& that)
	: m_pTarget(that.ptr())
{
	if (m_pTarget)
		m_pTarget->addHandle();
}

template<class T>
template<class S>
Handle<S> Handle<T>::cast()
{
	return Handle<S>(dynamic_cast<S*>(m_pTarget));
}

template<class T>
template<class S>
bool Handle<T>::operator == (const Handle<S>& that) const
{
	return m_pTarget == that.ptr();
}

template<class T>
template<class S>
bool Handle<T>::operator != (const Handle<S>& that) const
{
	return m_pTarget != that.ptr();
}

template<class T>
template<class S>
bool Handle<T>::operator < (const Handle<S>& that) const
{
	return m_pTarget < that.ptr();
}

template<class T>
template<class S>
bool Handle<T>::operator > (const Handle<S>& that) const
{
	return m_pTarget > that.ptr();
}

template<class T>
template<class S>
bool Handle<T>::operator <= (const Handle<S>& that) const
{
	return m_pTarget <= that.ptr();
}

template<class T>
template<class S>
bool Handle<T>::operator >= (const Handle<S>& that) const
{
	return m_pTarget >= that.ptr();
}

template<class T>
Handle<T>::~Handle()
{
	if (m_pTarget)
		m_pTarget->removeHandle();
}

template<class T>
T&
Handle<T>::operator*() const
{
	return *m_pTarget;
}

template<class T>
T*
Handle<T>::operator->() const
{
	return m_pTarget;
}

template<class T>
Handle<T>&
Handle<T>::operator=(const Handle<T>& that)
{
	if (m_pTarget == that.m_pTarget)
		return (*this);

	if (m_pTarget)
		m_pTarget->removeHandle();

	m_pTarget = that.m_pTarget;

	if (m_pTarget)
		m_pTarget->addHandle();

	return (*this);
}


template<class T>
bool
Handle<T>::isSet() const
{
	return (m_pTarget != NULL);
}

template<class T>
bool
Handle<T>::isNull() const
{
	return (m_pTarget == NULL);
}

template<class T>
void
Handle<T>::setNull()
{
	if (m_pTarget)
	{
		m_pTarget->removeHandle();
		m_pTarget = NULL;
	}
}
