// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "FileIndexInfo.h"

class FIFile;
class FIContent;

class FIIndex
{
public:
	FIIndex();
	void setFileIndexInfo(const FileIndexInfo& info);
	void addFIFile(FIFile* pFile);
	void setFIContent(FIContent* pContent);

	UInt32 getLink();
	UTF16 getLinkShortString();
	UTF16 getLinkLongString();
	
	UInt32 getContent();
	UTF16 getContentShortString();
	UTF16 getContentLongString();

	void setLinkGroup(UInt32& iNextLinkGroup);

private:
	FileIndexInfo m_info;
	UInt32 m_iLinkGroup;
	Array<FIFile*> m_pFIFiles;
	FIContent* m_pContent;
};
