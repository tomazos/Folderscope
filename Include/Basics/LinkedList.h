// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics/Ascii.h"
#include "Basics/Blob.h"
#include "Basics/UTF16.h"
#include "Basics/UTF8.h"

template <class T>
class LinkedList : public Handleable
{
public:
	LinkedList();
	~LinkedList();

	void add(const T&);
	bool empty();
	Size size();

	class Iterator
	{
	public:
		Iterator();
		Iterator(const LinkedList<T>&);
		bool operator () (T&);
	private:
		void* m_pCurrentNode;
		const LinkedList<T>* m_pTarget;
	};

private:
	class Node
	{
	public:
		Node(const T& value);

		T m_value;
		Node* m_pNext;
	};

	Size m_iSize;
	Node* m_pHead;
	Node* m_pTail;

	LinkedList(const LinkedList<T>&) {}; 
	LinkedList<T>& operator=(const LinkedList<T>&) { return *this; };
};

#define DeclBasicTypeList(BasicType) \
	typedef LinkedList<BasicType> BasicType##List; \
	typedef Handle<BasicType##List> H##BasicType##List;

DeclBasicTypeList(Ascii);
DeclBasicTypeList(Blob);
DeclBasicTypeList(UTF16);
DeclBasicTypeList(UTF8);

template <class T>
LinkedList<T>::Node::Node(const T& value)
    : m_value(value)
	, m_pNext(NULL)
{

}

template <class T>
LinkedList<T>::LinkedList()
	: m_pHead(NULL)
	, m_pTail(NULL)
	, m_iSize(0)
{
}

template <class T>
LinkedList<T>::~LinkedList()
{
	Node* p = m_pHead;
	while (p != NULL)
	{
		Node* t = p;
		p = p->m_pNext;
		DelObject(t);
		t = NULL;
	}
}

template <class T>
void LinkedList<T>::add(const T& value)
{
	m_iSize++;

	if (m_pHead == NULL)
	{
		m_pHead = NewObject(Node, value);
		m_pTail = m_pHead;
	}
	else
	{
		m_pTail->m_pNext = NewObject(Node, value);
		m_pTail = m_pTail->m_pNext;
	}
}

template <class T>
bool LinkedList<T>::empty()
{
	return m_pHead == NULL;
}

template <class T>
Size LinkedList<T>::size()
{
	return m_iSize;
}

template <class T>
LinkedList<T>::Iterator::Iterator()
	: m_pTarget(NULL)
	, m_pCurrentNode(NULL)
{

}

template <class T>
LinkedList<T>::Iterator::Iterator(const LinkedList<T> & target)
	: m_pTarget(&target)
	, m_pCurrentNode(NULL)
{

}

template <class T>
bool LinkedList<T>::Iterator::operator ()(T & value)
{
	if (m_pTarget == NULL)
		return false;

	typedef Node* PNode;
	PNode& p = *((Node**) &m_pCurrentNode);

	if (p == NULL)
		p = m_pTarget->m_pHead;
	else
		p = p->m_pNext;

	if (p == NULL)
		return false;
	else
		value = p->m_value;
	
	return true;
}
