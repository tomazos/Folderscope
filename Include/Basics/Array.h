// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Handleable.h"
#include <vector>
#include <algorithm>

template <class T, class Comparator>
class StrictWeakOrdering_
{
public:
	Comparator& m_comparator;
	StrictWeakOrdering_(Comparator& comparator)
		: m_comparator(comparator)
	{}

	bool operator()(const T& a, const T& b)
	{
		return m_comparator(a,b) < 0;
	}
};

template <class T>
class Array : public Handleable
{
public:
	Array() {};
	Array(Size iSize) : m_v(iSize) {}
	Array(const Array<T>& that) : m_v(that.m_v) {}

	T* ptr() { return &m_v[0]; }
	T& operator[] (Size iIndex)
	{
		if (iIndex >= size())
			m_v.resize(iIndex+1);
		
		return m_v[iIndex];
	}

	bool empty() { return m_v.empty(); }
	Size size() const { return m_v.size(); }

	void resize(Size i) { m_v.resize(i); }
	
	T& peek() { Check(!empty()); return m_v.back(); }
	void push(const T& t) { m_v.push_back(t); }
	T pop() { Check(!empty()); T t = m_v.back(); m_v.pop_back(); return t; }

	void insert(Size i, const T& t) { m_v.insert(m_v.begin() + i, t); }

	void del(Size i) { m_v.erase(m_v.begin() + i); }

	bool inbounds(Size i) { return i >= 0 && i < size(); }

	template<class C>
	void sort(C comparator)
	{

		std::stable_sort(m_v.begin(), m_v.end(), StrictWeakOrdering_<T,C>(comparator));
	}

private:
	std::vector<T> m_v;
};
