// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Check.h"
#include "Basics/DefaultHasher.h"

template<class K, class V, class H = DefaultHasher<K> >
class HashMap : public Handleable
{
public:
	HashMap(Size iTableSize);
	~HashMap();

	bool empty() const;
	Size size() const;

	bool has(const K& key) const;

	void add(const K& key, const V& value);

	V& viv(const K& key);

	V get(const K& key) const;
	void set(const K& key, const V& value);
	void del(const K& key);

	// HashMap<K,V,H>::Iterator iterate(myHashTable);
	// K k; V v; while(iterate(k,v)) process(k,v);
	class Iterator
	{
	public:
		Iterator(const HashMap<K,V,H>&);

		bool operator()(K& key, V& value);

	private:
		Size m_iCurrentBucket;
		void* m_pCurrentNode;

		const HashMap<K,V,H>& m_target;
	};

private:

	class Node
	{
	public:

		Node(const K& k, const V& v);

		Node* m_pNext;
		K m_key;
		V m_value;
	};

	typedef Node* PNode;

	PNode* m_ppBucketHeads;
	Size m_iNumBuckets;
	Size m_iHashMask;

	Size hash(const K& k) const;

	// uncopyable, unassignable
	HashMap(const HashMap&) {};
	HashMap& operator= (const HashMap&) {};
};

template <class K, class V, class H>
HashMap<K,V,H>::Iterator::Iterator(const HashMap<K,V,H>& target)
	: m_target(target)
	, m_iCurrentBucket(target.m_iNumBuckets)
	, m_pCurrentNode(NULL)
{

}

template <class K, class V, class H>
bool HashMap<K,V,H>::Iterator::operator() (K& key, V& value)
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

	key = pCurrentNode->m_key;
	value = pCurrentNode->m_value;

	return true;
}

template <class K, class V, class H>
bool HashMap<K,V,H>::empty() const
{
	for (int i = 0; i < m_iNumBuckets; i++)
		if (m_ppBucketHeads[i])
			return false;

	return true;
}

template <class K, class V, class H>
Size HashMap<K,V,H>::size() const
{
	int n = 0;

	for (int i = 0; i < m_iNumBuckets; i++)
		for (Node* p = m_ppBucketHeads[i]; p != NULL; p = p->m_pNext)
			n++;

	return n;
}

template <class K, class V, class H>
HashMap<K,V,H>::HashMap(Size iTableSize)
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

template <class K, class V, class H>
HashMap<K,V,H>::~HashMap()
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

template <class K, class V, class H>
Size HashMap<K,V,H>::hash(const K& k) const
{
	return H::hashf(k) & m_iHashMask;
}

template <class K, class V, class H>
bool HashMap<K,V,H>::has(const K& key) const
{
	for (Node* p = m_ppBucketHeads[hash(key)]; p != NULL; p = p->m_pNext)
		if (p->m_key == key)
			return true;

	return false;
}

template <class K, class V, class H>
V& HashMap<K,V,H>::viv(const K& key)
{
	Node** pp = &m_ppBucketHeads[hash(key)];

	for (; (*pp) != NULL; pp = &((*pp)->m_pNext))
		if ((*pp)->m_key == key)
			return (*pp)->m_value;

	(*pp) = NewObject(Node, key, V());
	return (*pp)->m_value;
}

template <class K, class V, class H>
void HashMap<K,V,H>::add(const K& key, const V& value)
{
	Node** pp = &m_ppBucketHeads[hash(key)];

	for (; (*pp) != NULL; pp = &((*pp)->m_pNext))
		if ((*pp)->m_key == key)
		{
			Check();
			return;
		}

	(*pp) = NewObject(Node, key, value);
}

template <class K, class V, class H>
void HashMap<K,V,H>::set(const K& key, const V& value)
{
	for (Node* p = m_ppBucketHeads[hash(key)]; p != NULL; p = p->m_pNext)
		if (p->m_key == key)
		{
			p->m_value = value;
			return;
		}

	Check();
	return;
}

template <class K, class V, class H>
V HashMap<K,V,H>::get(const K& key) const
{
	for (Node* p = m_ppBucketHeads[hash(key)]; p != NULL; p = p->m_pNext)
		if (p->m_key == key)
			return p->m_value;

	Check();
	return V();
}

template <class K, class V, class H>
void HashMap<K,V,H>::del(const K& key)
{
	for (Node** pp = &(m_ppBucketHeads[hash(key)]); (*pp) != NULL; pp = &((*pp)->m_pNext))
		if ((*pp)->m_key == key)
		{
			Node* pNext = (*pp)->m_pNext;
			DelObject(*pp);
			(*pp) = NULL;
			(*pp) = pNext;
			return;
		}

	Check();
	return;
}

template <class K, class V, class H>
HashMap<K,V,H>::Node::Node(const K& k, const V& v)
	: m_pNext(NULL)
	, m_key(k)
	, m_value(v)
{

}
