// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

class FIItem
{
public:
	typedef Array<UTF16> Components;
	typedef Handle<Components> HComponents;

	HComponents getComponents();
	Path getAbsolutePath(Error& error, const UTF16& sRoot);
	UTF16 getPath();
	UTF16 getBasename();

	UTF16 getExt();
	UTF16 getExtShortString();
	UTF16 getExtLongString();
	
	virtual UInt32 getLink() = 0;
	virtual UTF16 getLinkShortString() = 0;
	virtual UTF16 getLinkLongString() = 0;
	
	virtual UInt32 getContent() = 0;
	virtual UTF16 getContentShortString() = 0;
	virtual UTF16 getContentLongString() = 0;
	
	UInt64 getSize();
	UTF16 getSizeShortString();
	UTF16 getSizeLongString();
	
	TimePoint getCreated();
	UTF16 getCreatedShortString();
	UTF16 getCreatedLongString();
	
	TimePoint getModified();
	UTF16 getModifiedShortString();
	UTF16 getModifiedLongString();

	HComponents m_components;
	UTF16 m_sExt, m_sExtShort, m_sExtLong;
	UInt64 m_iFileSize;
	TimePoint m_created;
	TimePoint m_modified;

	virtual void mark() = 0;
	virtual void toggleMark() = 0;
	virtual void unmark() = 0;

	virtual Real64 getMarkedState() = 0;

protected:
	FIItem(HComponents, UTF16 sExt, UInt64 iFileSize, TimePoint created, TimePoint modified);
};
