// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"
#include "ProgressReport.h"
#include "TaskRepeater.h"

class FIItem;

DeclWeakHandleable(SurfaceScanner);
class SurfaceScanner : public WeakHandleable
{
public:
	class Master
	{
	public:
		virtual FIItem* SurfaceScanner_startFolder(const Path& folder, Array<UTF16>& relative) = 0;
		virtual void SurfaceScanner_skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage) = 0;
		virtual void SurfaceScanner_processFile(Error& error, const Path& file, const Array<UTF16>& relative) = 0;
		virtual void SurfaceScanner_progress(const ProgressReport&) = 0;
		virtual void SurfaceScanner_endFolder(FIItem*) = 0;
	};

	SurfaceScanner(Master& master, const Path& root);

	void tick(bool& bComplete);

private:
	bool m_bSendProgress;
	TaskRepeater m_repeater;
	void startRepeater();
	void stopRepeater();
public: void flagSendProgress(); private:

	void teardown();
	void setup();
	void work();

	void startFolder(Error& error);
	void processSubpath(const UTF16& sSubpath);
	void processFile(Error& error);
	void endFolder();

	FIItem* startFolder(const Path& folder, Array<UTF16>& relative);
	void skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage);
	void processFile(Error& error, const Path& file, Array<UTF16>& relative);
	void progressReport(const Path& path);
	void endFolder(FIItem*);

	enum ESurfaceScannerState
	{
		eInitial,
		eWorking,
		eComplete
	};

	ESurfaceScannerState m_eState;
	Path m_root;
	Master& m_master;

	Array<FIItem*> m_ancestor;
	Array<Path> m_absolute;
	Array<UTF16> m_relative;
	Array<HUTF16List> m_subpaths;
	Array<UTF16List::Iterator> m_subpath_iterator;
	Array<Real64> m_progressunit;
	Real64 m_fProgress;
	ProgressReport m_report;
};
