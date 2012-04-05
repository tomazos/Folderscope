// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

#include "FIComparator.h"
#include "FIItem.h"
#include "FIChange.h"
#include "ProgressReport.h"

DeclWeakHandleable(FolderImage);

class FolderImage : public WeakHandleable
{
public:
	enum EState
	{
		eInitial,
		eWorking,
		eIdle,
		eShutdown
	};

	class Listener
	{
	public:
		virtual void FolderImage_enteringState(EState) = 0;
		virtual void FolderImage_leavingState(EState) = 0;
		virtual void FolderImage_progress(const ProgressReport&) = 0;
		virtual void FolderImage_info(UTF16 sInfo) = 0;
		virtual void FolderImage_warning(UTF16 sError) = 0;
		virtual void FolderImage_fatal(UTF16 sError) = 0;
		virtual void FolderImage_quickJobStart() = 0;
		virtual void FolderImage_quickJobEnd() = 0;
	};

	virtual void addListener(Listener*) = 0;
	virtual void removeListener(Listener*) = 0;

	static HFolderImage create(const Path& root);
	static HFolderImage load(HInputStream);

	virtual Path getRoot() = 0;
	virtual UTF16 getShortRoot() = 0;

	virtual void save(HOutputStream) = 0;

	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void refresh() = 0;
	virtual void stop() = 0;
	virtual void shutdown() = 0;
	
	virtual Size getNoOfItems() = 0;
	virtual FIItem* getItem(Size iIndex) = 0;

	virtual void sortStatus(FICriteria, int&, FICention&) = 0;
	virtual void sort(FICriteria, FICention) = 0;

	virtual void deepScan() = 0;

	virtual void toggleMarkItem(Size iIndex) = 0;
	virtual void markAll() = 0;
	virtual void markNone() = 0;
	virtual void markInvert() = 0;
	virtual void markDuplicates() = 0;
	virtual void markMatchPath() = 0;
	virtual void markMatchContent() = 0;

	virtual void deleteMarked() = 0;
	virtual void renameMarked(UTF16 sSource, UTF16 sDestination) = 0;
	virtual void copyMarked(UTF16 sSource, UTF16 sDestination) = 0;

	virtual void externalCopy(UTF16 sAbsoluteExternalRoot) = 0;
	virtual void externalMove(UTF16 sAbsoluteExternalRoot) = 0;

	virtual bool areChangesPending() = 0;
	virtual Size getNoOfChanges() = 0;
	virtual FIChange* getChange(Size iIndex) = 0;
	virtual void removeChange(FIChange*) = 0;
	virtual void commitChanges() = 0;

	virtual ~FolderImage() {}
};
