// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"
#include "ProgressReport.h"
#include "TaskRepeater.h"

#include "FIDatabase.h"
#include "FileIndexInfo.h"

class FIItem;

DeclWeakHandleable(DeepScanner);
class DeepScanner : public WeakHandleable
{
public:
	class Master
	{
	public:
		virtual void DeepScanner_skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage) = 0;
		virtual void DeepScanner_progress(const ProgressReport&) = 0;
	};

	DeepScanner(Master& master, const Path& root, FIDatabase& db);

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

	void calculateGroups();

	enum EDeepScannerState
	{
		eInitial,
		eWorking,
		eComplete
	};

	EDeepScannerState m_eState;
	Path m_root;
	Master& m_master;
	FIDatabase& m_db;
	int m_iCurItem;

	HashMap<FileIndexInfo, FIIndex*> m_fiimap;
	HashMap<Blob, FIContent*> m_shamap;

	void processFile(FIFile* pFile);
	FIIndex* vivFileIndex(Error& error, FileInputStream& fis);
	FileIndexInfo getFileIndexInfo(Error& error, FileInputStream& fis);
	FIContent* vivFileContent(Error& error, FileInputStream& fis);
	Blob getFileSHA(Error& error, FileInputStream& fis);

	void skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage);
	void progressReport(const Path& path);

	Real64 m_fProgress;
	ProgressReport m_report;
};
