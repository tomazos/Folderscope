// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Lock.h"

#include "Basics/Handle.h"
#define DeclHandleable(T) class T; typedef Handle<T> H##T

class Handleable
{
public:
	inline Handleable();
	inline Handleable(const Handleable& that);
	inline Handleable& operator=(const Handleable& that);
	inline virtual ~Handleable();

	inline bool isHandled();

	void addHandle();
	void removeHandle();

private:
	volatile LONG m_iNumHandles;
};

inline Handleable::Handleable()
	: m_iNumHandles(0)
{
}

inline Handleable::Handleable(const Handleable& that)
	: m_iNumHandles(0)
{
}

inline Handleable& Handleable::operator=(const Handleable& that)
{
	return (*this);
}

inline Handleable::~Handleable()
{
}

inline bool Handleable::isHandled()
{
	return m_iNumHandles > 0;
}
