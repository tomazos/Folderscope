// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Task.h"
#include "Basics/Handle.h"
#include "Basics/WeakHandle.h"

template<class C, class F>
class MethodTask0 : public Task
{
public:
	MethodTask0(C* c, F f) : m_c(Handle<C>(c)), m_f(f) {}
	
	virtual void execute()
	{
		Handle<C> c(m_c);
		if (c.isSet())
		{
			((c.ptr())->*m_f)();
		}
	}
	WeakHandle<C> m_c;
	F m_f;
};
template<class C, class F> HTask buildMethodTask(C* c, F f)
{
//	return (new MethodTask0<C, F> (c,f));
	return NewObject(TemplateInst(MethodTask0, C, F), c, f);
}

template<class C, class F, class P1>
class MethodTask1 : public Task
{
public:
	MethodTask1(C* c, F f, P1 p1) : m_c(Handle<C>(c)), m_f(f), m_p1(p1) {}
	
	virtual void execute()
	{
		Handle<C> c(m_c);
		if (c.isSet())
		{
			((c.ptr())->*m_f)(m_p1);
		}
	}
	WeakHandle<C> m_c;
	F m_f; P1 m_p1;
};
template<class C, class F, class P1>
HTask buildMethodTask(C* c, F f, P1 p1) { return NewObject(TemplateInst(MethodTask1, C, F, P1), c, f, p1); }

template<class C, class F, class P1, class P2>
class MethodTask2 : public Task
{
public:
	MethodTask2(C* c, F f, P1 p1, P2 p2) : m_c(Handle<C>(c)), m_f(f), m_p1(p1), m_p2(p2) {}
	
	virtual void execute()
	{
		Handle<C> c(m_c);
		if (c.isSet())
		{
			((c.ptr())->*m_f)(m_p1, m_p2);
		}
	}
	WeakHandle<C> m_c;
	F m_f; P1 m_p1; P2 m_p2;
};
template<class C, class F, class P1, class P2>
HTask buildMethodTask(C* c, F f, P1 p1, P2 p2) { return NewObject(TemplateInst(MethodTask2, C, F, P1, P2), c, f, p1, p2); }
