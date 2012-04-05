// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "FIItem.h"

class FIDir : public FIItem
{
public:
	FIDir(FIDir* pParent, HComponents components);

	bool hasChildren() { return !m_pChildren.empty(); }

	void addChild(FIItem* fi);

	void addDescendant(FIItem* fi);

	void detachFromParent();

	virtual void mark();
	virtual void toggleMark();
	virtual void unmark();

	virtual Real64 getMarkedState();

	void markDescendant();
	void unmarkDescendant();

	virtual UInt32 getLink();
	virtual UTF16 getLinkShortString();
	virtual UTF16 getLinkLongString();
	
	virtual UInt32 getContent();
	virtual UTF16 getContentShortString();
	virtual UTF16 getContentLongString();

private:
	void removeChild(FIItem*);

	FIDir* m_pParent;

	TreeSet<FIItem*> m_pChildren;

	int m_iNumFiles;
	int m_iNumMarkedFiles;
};
