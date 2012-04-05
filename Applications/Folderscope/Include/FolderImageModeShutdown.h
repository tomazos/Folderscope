// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "FolderImageMode.h"

class FolderImageModeShutdown : public FolderImageMode
{
public:
	FolderImageModeShutdown(FolderImageImpl* pImage) : FolderImageMode(pImage) {}
	virtual const Chr16* getModeName() { return L"shutdown"; }
};
