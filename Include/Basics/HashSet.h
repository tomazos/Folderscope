// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/DefaultHasher.h"

template<class T, class H = DefaultHasher<T> >
class HashSet
{
public:
	HashSet(Size iTableSize);
	~HashSet();

	bool empty() const;
	Size size() const;

	inline void operator+= (const T&);
	inline void operator-= (const T&);
	
	inline bool operator[] (const T&) const;

	class Iterator
	{
	public:
		Iterator(const HashSet<T,H>&);

		bool operator()(T&);

	private:
		Size m_iCurrentBucket;
		void* m_pCurrentNode;

		const HashSet<T,H>& m_target;
	};

private:

	class Node
	{
	public:

		Node(const T&);

		Node* m_pNext;
		T m_val;
	};

	typedef Node* PNode;

	PNode* m_ppBucketHeads;
	Size m_iNumBuckets;
	Size m_iHashMask;

	Size hash(const T&) const;

	// uncopyable, unassignable
	HashSet(const HashSet&) {};
	HashSet& operator= (const HashSet&) {};
};

template <class T, class H>
HashSet<T,H>::Iterator::Iterator(const HashSet<T,H>& target)
	: m_target(target)
	, m_iCurrentBucket(target.m_iNumBuckets)
	, m_pCurrentNode(NULL)
{

}

template <class T, class H>
bool HashSet<T,H>::Iterator::operator() (T& val)
{
	PNode& pCurrentNode = *((Node**) &m_pCurrentNode);

	if (m_iCurrentBucket == m_target.m_iNumBuckets)
	{
		m_iCurrentBucket = 0;
		pCurrentNode = m_target.m_ppBucketHeads[0];
	}
	else
		pCurrentNode = pCurrentNode->m_pNext;

	while (pCurrentNode == NULL)
	{
		if (++m_iCurrentBucket == m_target.m_iNumBuckets)
			return false;

		pCurrentNode = m_target.m_ppBucketHeads[m_iCurrentBucket];
	}

	val = pCurrentNode->m_val;

	return true;
}

template <class T, class H>
bool HashSet<T,H>::empty() const
{
	for (int i = 0; i < m_iNumBuckets; i++)
		if (m_ppBucketHeads[i])
			return false;

	return true;
}

template <class T, class H>
Size HashSet<T,H>::size() const
{
	int n = 0;

	for (int i = 0; i < m_iNumBuckets; i++)
		for (Node* p = m_ppBucketHeads[i]; p != NULL; p = p->m_pNext)
			n++;

	return n;
}

template <class T, class H>
HashSet<T,H>::HashSet(Size iTableSize)
{
	m_iNumBuckets = 2;
	
	while (m_iNumBuckets < iTableSize)
	{
		m_iNumBuckets <<= 1;
	}

	m_iHashMask = m_iNumBuckets - 1;

	m_ppBucketHeads = NewArray(PNode, m_iNumBuckets);

	Memory::zero(m_ppBucketHeads, m_iNumBuckets * sizeof(PNode));
}

template <class T, class H>
HashSet<T,H>::~HashSet()
{
	for (int i = 0; i < m_iNumBuckets; i++)
	{
		Node* p = m_ppBucketHeads[i];

		while (p != NULL)
		{
			Node* n = p->m_pNext;
			DelObject(p);
			p = NULL;
			p = n;
		}
	}

	DelArray(m_ppBucketHeads);
	m_ppBucketHeads = NULL;
}

template <class T, class H>
Size HashSet<T,H>::hash(const T& t) const
{
	return H::hashf(t) & m_iHashMask;
}

template<class T, class H>
inline void
HashSet<T,H>::operator+= (const T& val)
{
	Node** pp = &m_ppBucketHeads[hash(val)];

	for (; (*pp) != NULL; pp = &((*pp)->m_pNext))
		if ((*pp)->m_val == val)
			return;

	(*pp) = NewObject(Node, val);
}

template<class T, class H>
inline void
HashSet<T,H>::operator-= (const T& val)
{
	for (Node** pp = &(m_ppBucketHeads[hash(val)]); (*pp) != NULL; pp = &((*pp)->m_pNext))
		if ((*pp)->m_val == val)
		{
			Node* pNext = (*pp)->m_pNext;
			DelObject(*pp);
			(*pp) = NULL;
			(*pp) = pNext;
			return;
		}
}

template<class T, class H>
inline bool HashSet<T,H>::operator[] (const T& val) const
{
	for (Node* p = m_ppBucketHeads[hash(val)]; p != NULL; p = p->m_pNext)
		if (p->m_val == val)
			return true;

	return false;
}

template <class T, class H>
HashSet<T,H>::Node::Node(const T& val)
	: m_pNext(NULL)
	, m_val(val)
{

}
