// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/DefaultComparator.h"
#include "Basics/Check.h"

template <class K, class V, class C = DefaultComparator<K>>
class TreeMap
{
public:
	TreeMap();
	~TreeMap();

	bool empty() const;
	Size size() const;

	bool has(const K& key) const;

	V& viv(const K& key);

	void add(const K& key, const V& value);
	
	V get(const K& key) const;
	void set(const K& key, const V& value);
	void del(const K& key);

	// TreeMap<K,V,C>::Iterator iterate(myhash);
	// K k; V v; while(iterate(k,v)) process(k,v);
	class Iterator
	{
	public:
		Iterator(const TreeMap<K,V,C>&);

		bool operator()(K& key, V& value);

	private:
		void* m_pCurrentNode;

		const TreeMap<K,V,C>& m_target;
	};

private:

	class Node
	{
	public:

		Node(const K& k, const V& v, Node* parent);

		Node* m_pLeftChild;
		Node* m_pRightChild;
		Node* m_pParent;
		K m_key;
		V m_value;
	};

	Node* m_pRoot;

	void removeNode(Node* p);

	// uncopyable, unassignable
	TreeMap(const TreeMap&) {};
	TreeMap& operator= (const TreeMap&) {};

};

template<class K, class V, class C>
bool TreeMap<K,V,C>::empty() const
{
	return !m_pRoot;
}

template <class K, class V, class C>
TreeMap<K,V,C>::Node::Node(const K& k, const V& v, Node* pParent)
	: m_pLeftChild(NULL)
	, m_pRightChild(NULL)
	, m_pParent(pParent)
	, m_key(k)
	, m_value(v)
{

}

template <class K, class V, class C>
TreeMap<K,V,C>::TreeMap()
	: m_pRoot(0)
{
	
}

template <class K, class V, class C>
TreeMap<K,V,C>::~TreeMap()
{
	Array<Node*> trace;

	trace.push(m_pRoot);

	while (!trace.empty())
	{
		Node* p = trace.pop();
		if (p)
		{
			trace.push(p->m_pLeftChild);
			trace.push(p->m_pRightChild);
			DelObject(p);
		}
	}
}

template <class K, class V, class C>
Size TreeMap<K,V,C>::size() const
{
	Size n = 0;

	Node* p = m_pRoot;

	int side = 1;

	while (p != NULL)
	{
		switch (side)
		{
		case 1:
			n++;
			if (p->m_pLeftChild)
			{
				p = p->m_pLeftChild;
				break;
			}
		case 2:
			if (p->m_pRightChild)
			{
				p = p->m_pRightChild;
			    side = 1;
				break;
			}
		case 3:
			if (p->m_pParent)
			{
				if (p == p->m_pParent->m_pLeftChild)
					side = 2;
				else if (p == p->m_pParent->m_pRightChild)
					side = 3;
				else
				{
					Check();
					return 0;
				}
			}

			p = p->m_pParent;
			break;
		default:
			{
				Check();
				return 0;
			}
		}
	}

	return n;
}

template <class K, class V, class C>
bool TreeMap<K,V,C>::has(const K& key) const
{
	Node* p = m_pRoot;

	while (p != NULL)
	{
		SInt64 iComp = C::comparef(key, p->m_key);

		if (iComp == 0)
			return true;
		else if (iComp < 0)
			p = p->m_pLeftChild;
		else // iComp > 0
			p = p->m_pRightChild;
	}

	return false;
}

template <class K, class V, class C>
V& TreeMap<K,V,C>::viv(const K& key)
{
	Node* parent = NULL;
	Node** pp = &m_pRoot;

	while ((*pp) != NULL)
	{
		SInt64 iComp = C::comparef(key, (*pp)->m_key);

		if (iComp == 0)
			return (*pp)->m_value;
		else if (iComp < 0)
		{
			parent = (*pp);
			pp = &(parent->m_pLeftChild);
		}
		else // iComp > 0
		{
			parent = (*pp);
			pp = &(parent->m_pRightChild);
		}
	}

	(*pp) = NewObject(Node, key, V(), parent);

	return (*pp)->m_value;
}

template <class K, class V, class C>
void TreeMap<K,V,C>::add(const K& key, const V& value)
{
	Node* parent = NULL;
	Node** pp = &m_pRoot;

	while ((*pp) != NULL)
	{
		SInt64 iComp = C::comparef(key, (*pp)->m_key);

		if (iComp == 0)
		{
			Check();
			return;
		}
		else if (iComp < 0)
		{
			parent = (*pp);
			pp = &(parent->m_pLeftChild);
		}
		else // iComp > 0
		{
			parent = (*pp);
			pp = &(parent->m_pRightChild);
		}
	}

	(*pp) = NewObject(Node, key, value, parent);
}

template <class K, class V, class C>
V TreeMap<K,V,C>::get(const K& key) const
{
	Node* p = m_pRoot;

	while (p != NULL)
	{
		SInt64 iComp = C::comparef(key, p->m_key);

		if (iComp == 0)
			return p->m_value;
		else if (iComp < 0)
			p = p->m_pLeftChild;
		else // iComp > 0
			p = p->m_pRightChild;
	}

	Check();
	return V();
}

template <class K, class V, class C>
void TreeMap<K,V,C>::set(const K& key, const V& value)
{
	Node* p = m_pRoot;

	while (p != NULL)
	{
		SInt64 iComp = C::comparef(key, p->m_key);

		if (iComp == 0)
		{
			p->m_value = value;
			return;
		}
		else if (iComp < 0)
			p = p->m_pLeftChild;
		else // iComp > 0
			p = p->m_pRightChild;
	}

	Check();
	return;
}

template <class K, class V, class C>
void TreeMap<K,V,C>::del(const K& key)
{
	Node* p = m_pRoot;

	while (p != NULL)
	{
		SInt64 iComp = C::comparef(key, p->m_key);

		if (iComp == 0)
		{
			removeNode(p);
			return;
		}
		else if (iComp < 0)
			p = p->m_pLeftChild;
		else // iComp > 0
			p = p->m_pRightChild;
	}

	Check();// (L"No such node");
	return;
}

template <class K, class V, class C>
void TreeMap<K,V,C>::removeNode(Node* p)
{
	if ((!p->m_pLeftChild) && (!p->m_pRightChild))
	{
		if (p->m_pParent)
		{
			if (p->m_pParent->m_pLeftChild == p)
				p->m_pParent->m_pLeftChild = NULL;
			else if (p->m_pParent->m_pRightChild == p)
				p->m_pParent->m_pRightChild = NULL;
			else
			{
				Check();// (L"TreeMap Remove node corruption #1");
				return;
			}
		}
		else
		{
			m_pRoot = 0;
		}

		DelObject(p);
	}
	else if (p->m_pLeftChild && (!p->m_pRightChild))
	{
		p->m_pLeftChild->m_pParent = p->m_pParent;
		if (p->m_pParent)
		{
			if (p->m_pParent->m_pLeftChild == p)
				p->m_pParent->m_pLeftChild = p->m_pLeftChild;
			else if (p->m_pParent->m_pRightChild == p)
				p->m_pParent->m_pRightChild = p->m_pLeftChild;
			else
			{
				Check();// (L"TreeMap Remove node corruption #2");
				return;
			}
		}
		else
		{
			m_pRoot = p->m_pLeftChild;
		}

		DelObject(p);
	}
	else if ((!p->m_pLeftChild) && p->m_pRightChild)
	{
		p->m_pRightChild->m_pParent = p->m_pParent;
		if (p->m_pParent)
		{
			if (p->m_pParent->m_pLeftChild == p)
				p->m_pParent->m_pLeftChild = p->m_pRightChild;
			else if (p->m_pParent->m_pRightChild == p)
				p->m_pParent->m_pRightChild = p->m_pRightChild;
			else
			{
				Check();// (L"TreeMap Remove node corruption #3");
				return;
			}
		}
		else
		{
			m_pRoot = p->m_pRightChild;
		}

		DelObject(p);
	}
	else if (p->m_pLeftChild && p->m_pRightChild)
	{
		Node* pSuccessor = p->m_pRightChild;
		while (pSuccessor->m_pLeftChild)
			pSuccessor = pSuccessor->m_pLeftChild;

		p->m_key = pSuccessor->m_key;
		p->m_value = pSuccessor->m_value;

		removeNode(pSuccessor);
	}
	else
	{
		Check();// (L"TreeMap Remove node corruption #4");
		return;
	}
}

template <class K, class V, class C>
TreeMap<K,V,C>::Iterator::Iterator(const TreeMap<K,V,C>& target)
    : m_target(target)
	, m_pCurrentNode(NULL)
{

}

template <class K, class V, class C>
bool TreeMap<K,V,C>::Iterator::operator()(K& key, V& value)
{
	typedef Node* PNode;
	PNode& p = *((Node**) &m_pCurrentNode);

	if (p == NULL)
	{
		p = m_target.m_pRoot;

		if (p == NULL)
			return false;

		while (p->m_pLeftChild)
			p = p->m_pLeftChild;
	}
	else
	{
		if (p->m_pRightChild)
		{
			p = p->m_pRightChild;
			while (p->m_pLeftChild)
				p = p->m_pLeftChild;
		}
		else
		{
			while (p->m_pParent && p == p->m_pParent->m_pRightChild)
				p = p->m_pParent;

			p = p->m_pParent;

			if (p == NULL)
				return false;
		}
	}

	key = p->m_key;
	value = p->m_value;
	return true;
}
