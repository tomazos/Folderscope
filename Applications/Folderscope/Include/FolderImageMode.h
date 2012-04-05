// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

class FolderImageImpl;
class FIChange;
#include "FIComparator.h"
#include "FIItem.h"

class FolderImageMode
{
public:
	FolderImageImpl* imag;

	FolderImageMode(FolderImageImpl* pImage);
	virtual const Chr16* getModeName() = 0;

	void warnNotImplemented(const Chr16* sFunction);

	virtual void work();
	virtual void save(HOutputStream);
	virtual void start();
	virtual void pause();
	virtual void refresh();
	virtual void stop();
	virtual void shutdown();
	virtual Size getNoOfItems();
	virtual FIItem* getItem(Size iIndex);
	virtual void sort(FICriteria, FICention);
	virtual void deepScan();
	virtual void toggleMarkItem(Size iIndex);
	virtual void markAll();
	virtual void markNone();
	virtual void markInvert();
	virtual void markDuplicates();
	virtual void markMatchPath();
	virtual void markMatchContent();
	virtual void deleteMarked();
	virtual void renameMarked(UTF16 sSource, UTF16 sDestination);
	virtual void copyMarked(UTF16 sSource, UTF16 sDestination);
	virtual void externalCopy(UTF16 sAbsoluteExternalRoot);
	virtual void externalMove(UTF16 sAbsoluteExternalRoot);
	virtual bool areChangesPending();
	virtual Size getNoOfChanges();
	virtual FIChange* getChange(Size iIndex);
	virtual void removeChange(FIChange*);
	virtual void commitChanges();
};