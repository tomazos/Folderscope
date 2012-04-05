// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "DeepScanner.h"

#include "FIFile.h"
#include "FIContent.h"
#include "FIIndex.h"

#include "Runtime.h"

DeepScanner::DeepScanner(Master& master, const Path& root, FIDatabase& db)
	: m_root(root)
	, m_master(master)
	, m_db(db)
	, m_iCurItem(0)
	, m_eState(eInitial)
	, m_fProgress(0)
	, m_bSendProgress(true)
	, m_fiimap(db.m_pFIFiles.size())
	, m_shamap(db.m_pFIFiles.size())
{}

void DeepScanner::tick(bool& bComplete)
{
	bComplete = false;

	switch (m_eState)
	{
	case eInitial: setup(); return;
	case eWorking: work(); return;
	case eComplete: teardown(); bComplete = true; return;
	}

	Check();
};

void DeepScanner::setup()
{
	startRepeater();

	class SkipRoot {};

	Error error;

	m_report.setTaskDescription(m_root);

	m_fProgress = 0;
	
	m_eState = eWorking;
};

void DeepScanner::work()
{
	if (m_iCurItem == m_db.m_pFIFiles.size())
	{
		m_eState = eComplete;
		return;
	}

	FIFile* p = m_db.m_pFIFiles[m_iCurItem];

	if (p != NULL)
		processFile(p);
	else
		Check();

	m_iCurItem++;

	if (m_iCurItem == m_db.m_pFIFiles.size())
		m_eState = eComplete;
}

void DeepScanner::processFile(FIFile* pFile)
{
	Error error;
	class SkipFile {};

	try
	{
		Path path = pFile->getAbsolutePath(error, m_db.m_root);
		if (error)
			throw SkipFile();

		m_fProgress = Real64(m_iCurItem) / Real64(m_db.m_pFIFiles.size());
		progressReport(path);

		FileInputStream fis(error, path);
		if (error)
			throw SkipFile();

		FIIndex* pIndex = vivFileIndex(error, fis);

		if (error)
			throw SkipFile();

		pFile->setFIIndex(pIndex);
		pIndex->addFIFile(pFile);
	}
	catch (SkipFile&)
	{
		UTF16 sErrorMessage = (error ? UTF16(error) : L"Unknown Error");

		skipWarning(pFile->getPath(), error);
	}
}

FIIndex* DeepScanner::vivFileIndex(Error& error, FileInputStream& fis)
{
	FileIndexInfo fii = getFileIndexInfo(error, fis);
	if (error)
		return NULL;

	typedef FIIndex* PFIIndex;
	PFIIndex& pIndex = m_fiimap.viv(fii);
	if (pIndex == NULL)
	{
		pIndex = NewObject(FIIndex);
		m_db.m_pFIIndexes.push(pIndex);
		pIndex->setFileIndexInfo(fii);

		FIContent* pContent = vivFileContent(error, fis);
		if (error)
			return NULL;
		pIndex->setFIContent(pContent);
		pContent->addFIIndex(pIndex);
	}

	return pIndex;
}

FileIndexInfo DeepScanner::getFileIndexInfo(Error& error, FileInputStream& fis)
{
	UInt32 iVolumeIndex, iFileIndexHi, iFileIndexLo;
	fis.getFileIndexInfo(error, iVolumeIndex, iFileIndexHi, iFileIndexLo);
	if (error)
		return FileIndexInfo();
	else
		return FileIndexInfo(iVolumeIndex, iFileIndexHi, iFileIndexLo);
}

FIContent* DeepScanner::vivFileContent(Error& error, FileInputStream& fis)
{
	Blob sha = getFileSHA(error, fis);
	if (error)
		return NULL;

	typedef FIContent* PFIContent;
	PFIContent& pContent = m_shamap.viv(sha);
	if (pContent == NULL)
	{
		pContent = NewObject(FIContent);
		m_db.m_pFIContents.push(pContent);
		pContent->setSHA(sha);
	}
	return pContent;
}

Blob DeepScanner::getFileSHA(Error& error, FileInputStream& fis)
{
	const int iBufSize = 4096;
	SHA512 sha;
	Buffer buf(iBufSize);
	while (true)
	{
		int iBytesRead = fis.read(error, buf.ptr(), iBufSize);
		if (error)
			return Blob();

		sha.write(buf.ptr(), iBytesRead);
		if (iBytesRead < iBufSize)
			return sha.finish();
	}
}

void DeepScanner::calculateGroups()
{
	UInt32 iNextLinkGroup = 1;
	for (int i = 0; i < m_db.m_pFIIndexes.size(); i++)
	{
		FIIndex* pIndex = m_db.m_pFIIndexes[i];
		pIndex->setLinkGroup(iNextLinkGroup);
	}

	UInt32 iNextContentGroup = 1;
	for (int i = 0; i < m_db.m_pFIContents.size(); i++)
	{
		FIContent* pContent = m_db.m_pFIContents[i];
		pContent->setContentGroup(iNextContentGroup);
	}
}

void DeepScanner::progressReport(const Path& path)
{
	if (m_bSendProgress)
	{
		m_bSendProgress = false;

		m_report.setProgress(m_fProgress);
		m_report.setSubtaskDescription(UTF16(path));
		m_master.DeepScanner_progress(m_report);
	}
}

void DeepScanner::teardown()
{
	m_repeater.stop();

	m_report.setProgress(0.9);
	m_report.setSubtaskDescription(L"Calculating content and link groups...");
	m_master.DeepScanner_progress(m_report);

	calculateGroups();

	m_report.setProgress(1.0);
	m_report.setSubtaskDescription(L"Finished...");
	m_master.DeepScanner_progress(m_report);
}

void DeepScanner::startRepeater()
{
	m_repeater.setTask(buildMethodTask(this, &DeepScanner::flagSendProgress));
	m_repeater.start();
}

void DeepScanner::stopRepeater()
{
	m_repeater.stop();
}

void DeepScanner::flagSendProgress()
{
	m_bSendProgress = true;
}

void DeepScanner::skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage)
{
	m_master.DeepScanner_skipWarning(sAbsolutePath, sErrorMessage);
}
