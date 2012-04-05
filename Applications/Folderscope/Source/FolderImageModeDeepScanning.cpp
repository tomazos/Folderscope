// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderImageModeDeepScanning.h"

#include "FolderImageImpl.h"
#include "FolderImageModeIdle.h"

void FolderImageModeDeepScanning::work()
{
	bool bComplete = false;
	imag->deepScan(bComplete);

	if (!bComplete)
		imag->work();
	else
	{
		imag->changeMode<FolderImageModeIdle>(FolderImage::eIdle);
	}
};
