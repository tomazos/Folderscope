// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "FolderImageMode.h"

class FolderImageModeIdle : public FolderImageMode
{
public:
	FolderImageModeIdle(FolderImageImpl* pImage) : FolderImageMode(pImage) {}
	virtual const Chr16* getModeName() { return L"idle"; }

	virtual Size getNoOfItems();
	virtual FIItem* getItem(Size iIndex);

	virtual void deepScan();

	virtual void toggleMarkItem(int iIndex);
	virtual void markAll();
	virtual void markNone();
	virtual void markInvert();

	virtual void shutdown();

	virtual void sort(FICriteria, FICention);
};
