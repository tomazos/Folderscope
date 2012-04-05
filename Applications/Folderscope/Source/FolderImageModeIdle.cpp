// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderImageModeIdle.h"

#include "FolderImageImpl.h"
#include "FolderImageModeShutdown.h"
#include "FolderImageModeDeepScanning.h"

#include "FIItem.h"

#include "Runtime.h"

Size FolderImageModeIdle::getNoOfItems()
{
	return imag->m_db.m_pFIItems.size();
}

FIItem* FolderImageModeIdle::getItem(Size iIndex)
{
	return imag->m_db.m_pFIItems[iIndex];
}

void FolderImageModeIdle::shutdown()
{
	imag->changeMode<FolderImageModeShutdown>(FolderImage::eShutdown);
}

void FolderImageModeIdle::sort(FICriteria crit, FICention cent)
{
	imag->FolderImage_quickJobStart();
	imag->m_comp.add(crit,cent);
	imag->m_db.m_pFIItems.sort(imag->m_comp);
	imag->FolderImage_quickJobEnd();
}

void FolderImageModeIdle::toggleMarkItem(int iIndex)
{
	if (imag->m_db.m_pFIItems.inbounds(iIndex))
	{
		imag->FolderImage_quickJobStart();
		imag->m_db.m_pFIItems[iIndex]->toggleMark();
		imag->FolderImage_quickJobEnd();
	}
}

void FolderImageModeIdle::deepScan()
{
	if (imag->m_db.m_pFIIndexes.size() == 0)
	{
		imag->work();
		imag->changeMode<FolderImageModeDeepScanning>(FolderImage::eWorking);
	}
	else
	{
		Check();
	}
}

void FolderImageModeIdle::markAll()
{
	imag->FolderImage_quickJobStart();
	for (int i = 0; i < imag->m_db.m_pFIFiles.size(); i++)
		imag->m_db.m_pFIFiles[i]->mark();
	imag->FolderImage_quickJobEnd();
}

void FolderImageModeIdle::markNone()
{
	imag->FolderImage_quickJobStart();
	for (int i = 0; i < imag->m_db.m_pFIFiles.size(); i++)
		imag->m_db.m_pFIFiles[i]->unmark();
	imag->FolderImage_quickJobEnd();
}

void FolderImageModeIdle::markInvert()
{
	imag->FolderImage_quickJobStart();
	for (int i = 0; i < imag->m_db.m_pFIFiles.size(); i++)
		imag->m_db.m_pFIFiles[i]->toggleMark();
	imag->FolderImage_quickJobEnd();
}
