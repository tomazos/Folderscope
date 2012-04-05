// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class FIIndex;

class FIContent
{
public:
	void setSHA(const Blob& sha) { m_sha = sha; }
	void addFIIndex(FIIndex* pIndex) { m_pFIIndexes.push(pIndex); }

	UInt32 getContent();
	UTF16 getContentShortString();
	UTF16 getContentLongString();

	void setContentGroup(UInt32& iNextContentGroup);

private:
	UInt32 m_iContentGroup;
	Blob m_sha;
	Array<FIIndex*> m_pFIIndexes;
};
