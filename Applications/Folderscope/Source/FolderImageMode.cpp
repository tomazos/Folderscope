// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderImageMode.h"

#include "FolderImageImpl.h"

FolderImageMode::FolderImageMode(FolderImageImpl* pImage)
	: imag(pImage)
{}

void FolderImageMode::warnNotImplemented(const Chr16* sFunction)
{
	imag->FolderImage_warning(UTF16::format(L"Folder image cannot \"%s\" in mode \"%s\". Ignoring.", sFunction, getModeName()));
}

void FolderImageMode::work()
{
	warnNotImplemented(L"work");
}

void FolderImageMode::save(HOutputStream)
{
	warnNotImplemented(L"save");
}

void FolderImageMode::start()
{
	warnNotImplemented(L"start");
}

void FolderImageMode::pause()
{
	warnNotImplemented(L"pause");
}

void FolderImageMode::refresh()
{
	warnNotImplemented(L"refresh");
}

void FolderImageMode::stop()
{
	warnNotImplemented(L"stop");
}

void FolderImageMode::shutdown()
{
	warnNotImplemented(L"shutdown");
}

Size FolderImageMode::getNoOfItems()
{
	return 0;
}

FIItem* FolderImageMode::getItem(Size iIndex)
{
	return NULL;
}

void FolderImageMode::sort(FICriteria, FICention)
{
	warnNotImplemented(L"sort");
}

void FolderImageMode::deepScan()
{
	warnNotImplemented(L"deepScan");
}

void FolderImageMode::toggleMarkItem(Size iIndex)
{
	warnNotImplemented(L"toggleMarkItem");
}

void FolderImageMode::markAll()
{
	warnNotImplemented(L"markAll");
}

void FolderImageMode::markNone()
{
	warnNotImplemented(L"markNone");
}

void FolderImageMode::markInvert()
{
	warnNotImplemented(L"markInvert");
}

void FolderImageMode::markDuplicates()
{
	warnNotImplemented(L"markDuplicates");
}

void FolderImageMode::markMatchPath()
{
	warnNotImplemented(L"markMatchPath");
}

void FolderImageMode::markMatchContent()
{
	warnNotImplemented(L"markMatchContent");
}

void FolderImageMode::deleteMarked()
{
	warnNotImplemented(L"deleteMarked");
}

void FolderImageMode::renameMarked(UTF16 sSource, UTF16 sDestination)
{
	warnNotImplemented(L"renameMarked");
}

void FolderImageMode::copyMarked(UTF16 sSource, UTF16 sDestination)
{
	warnNotImplemented(L"copyMarked");
}

void FolderImageMode::externalCopy(UTF16 sAbsoluteExternalRoot)
{
	warnNotImplemented(L"externalCopy");
}

void FolderImageMode::externalMove(UTF16 sAbsoluteExternalRoot)
{
	warnNotImplemented(L"externalMove");
}

bool FolderImageMode::areChangesPending()
{
	warnNotImplemented(L"areChangesPending");
	return false;
}

Size FolderImageMode::getNoOfChanges()
{
	warnNotImplemented(L"getNoOfChanges");
	return 0;
}

FIChange* FolderImageMode::getChange(Size iIndex)
{
	warnNotImplemented(L"getChange");
	return NULL;
}

void FolderImageMode::removeChange(FIChange*)
{
	warnNotImplemented(L"removeChange");
}

void FolderImageMode::commitChanges()
{
	warnNotImplemented(L"commitChanges");
}
