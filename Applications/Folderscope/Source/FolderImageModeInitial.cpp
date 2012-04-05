// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderImageModeInitial.h"

#include "FolderImageImpl.h"
#include "FolderImageModeSurfaceScanning.h"

void FolderImageModeInitial::start()
{
	if (imag->m_hLoadStream.isSet())
	{
		// TODO load
	} else {
		imag->work();
		imag->changeMode<FolderImageModeSurfaceScanning>(FolderImage::eWorking);
	}
}
