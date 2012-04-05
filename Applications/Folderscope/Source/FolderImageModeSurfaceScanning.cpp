// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderImageModeSurfaceScanning.h"

#include "FolderImageImpl.h"
#include "FolderImageModeIdle.h"

void FolderImageModeSurfaceScanning::work()
{
	bool bComplete = false;
	imag->surfaceScan(bComplete);

	if (!bComplete)
		imag->work();
	else
	{
		imag->changeMode<FolderImageModeIdle>(FolderImage::eIdle);
	}
};
