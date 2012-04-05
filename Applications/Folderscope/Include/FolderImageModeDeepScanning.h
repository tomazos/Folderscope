// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "FolderImageMode.h"

class FolderImageModeDeepScanning : public FolderImageMode
{
public:
	FolderImageModeDeepScanning(FolderImageImpl* pImage) : FolderImageMode(pImage) {}

	virtual const Chr16* getModeName() { return L"work"; }

	virtual void work();
};
