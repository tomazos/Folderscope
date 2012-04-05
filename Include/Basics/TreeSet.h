// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Check.h"
#include "Basics/DefaultComparator.h"

#include "Basics/Memory.h"
#include "Basics/Array.h"

template<class T, class C = DefaultComparator<T> >
class TreeSet
{
public:
	TreeSet();
	~TreeSet();

	bool empty() const;
	Size size() const;

	inline void clear();

	inline bool operator+= (const T&);
	inline bool operator-= (const T&);
	
	inline bool operator[] (const T&) const;

	class Iterator
	{
	public:
		Iterator(const TreeSet<T,C>&);

		bool operator()(T&);

	private:
		void* m_pCurrentNode;

		const TreeSet<T,C>& m_target;
	};

private:

	class Node
	{
	public:

		Node(const T& t, Node* parent);

		Node* m_pLeftChild;
		Node* m_pRightChild;
		Node* m_pParent;
		T m_val;
	};

	Node* m_pRoot;

	void removeNode(Node* p);

	// uncopyable, unassignable
	TreeSet(const TreeSet&) {};
	TreeSet& operator= (const TreeSet&) {};
};

template <class T, class C>
TreeSet<T,C>::Node::Node(const T& val, Node* pParent)
	: m_pLeftChild(NULL)
	, m_pRightChild(NULL)
	, m_pParent(pParent)
	, m_val(val)
{

}

template <class T, class C>
TreeSet<T,C>::TreeSet()
	: m_pRoot(0)
{
	
}

template <class T, class C>
void TreeSet<T,C>::clear()
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

	m_pRoot = NULL;
}

template <class T, class C>
TreeSet<T,C>::~TreeSet()
{
	clear();
}

template<class T, class C>
bool TreeSet<T,C>::empty() const
{
	return !m_pRoot;
}

template<class T, class C>
Size TreeSet<T,C>::size() const
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
					Check();// (L"TreeSet parent/child mismatch");
					return;
				}
			}

			p = p->m_pParent;
			break;
		default:
			{
				Check();// (L"TreeSet size > 3");
				return;
			}
		}
	}

	return n;
}

template<class T, class C>
inline bool
TreeSet<T,C>::operator+= (const T& val)
{
	Node* parent = NULL;
	Node** pp = &m_pRoot;

	while ((*pp) != NULL)
	{
		SInt64 iComp = C::comparef(val, (*pp)->m_val);

		if (iComp == 0)
			return false;
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

	(*pp) = NewObject(Node, val, parent);
	return true;
}

template<class T, class C>
inline bool
TreeSet<T,C>::operator-= (const T& val)
{
	Node* p = m_pRoot;

	while (p != NULL)
	{
		SInt64 iComp = C::comparef(val, p->m_val);

		if (iComp == 0)
		{
			removeNode(p);
			return true;
		}
		else if (iComp < 0)
			p = p->m_pLeftChild;
		else // iComp > 0
			p = p->m_pRightChild;
	}

	return false;
}

template<class T, class C>
inline bool
TreeSet<T,C>::operator[] (const T& val) const
{
	Node* p = m_pRoot;

	while (p != NULL)
	{
		SInt64 iComp = C::comparef(val, p->m_val);

		if (iComp == 0)
			return true;
		else if (iComp < 0)
			p = p->m_pLeftChild;
		else // iComp > 0
			p = p->m_pRightChild;
	}

	return false;
}

template <class T, class C>
TreeSet<T,C>::Iterator::Iterator(const TreeSet<T,C>& target)
    : m_target(target)
	, m_pCurrentNode(NULL)
{

}

template <class T, class C>
bool TreeSet<T,C>::Iterator::operator()(T& val)
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

	val = p->m_val;
	return true;
}

template <class T, class C>
void TreeSet<T,C>::removeNode(Node* p)
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
				Check();// (L"TreeSet removeNode 1");
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
				Check();// (L"TreeSet removeNode corruption #2");
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
				Check();// (L"TreeSet removeNode corruption #3");
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

		p->m_val = pSuccessor->m_val;

		removeNode(pSuccessor);
	}
	else
	{
		Check();// (L"TreeSet removeNode corruption #4");
		return;
	}
}
