// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Stream.h"

class FIItem;
class FIDir;
class FIFile;
class FIIndex;
class FIContent;

class FIDatabase
{
public:
	Path m_root;
	Array<FIItem*> m_pFIItems;
	Array<FIDir*> m_pFIDirs;
	Array<FIFile*> m_pFIFiles;
	Array<FIIndex*> m_pFIIndexes;
	Array<FIContent*> m_pFIContents;
};
