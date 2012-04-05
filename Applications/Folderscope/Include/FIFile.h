// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "FIItem.h"

class FIIndex;
class FIDir;

class FIFile : public FIItem
{
public:
	FIFile(FIDir* pParent, HComponents components, UTF16 sExt, UInt64 iFileSize, TimePoint created, TimePoint modified);

	virtual void mark();
	virtual void toggleMark();
	virtual void unmark();

	virtual Real64 getMarkedState();

	void setFIIndex(FIIndex* pIndex) { m_pIndex = pIndex; }

	virtual UInt32 getLink();
	virtual UTF16 getLinkShortString();
	virtual UTF16 getLinkLongString();
	
	virtual UInt32 getContent();
	virtual UTF16 getContentShortString();
	virtual UTF16 getContentLongString();

private:
	bool m_bMarked;

	FIDir* m_pParent;
	FIIndex* m_pIndex;
};
