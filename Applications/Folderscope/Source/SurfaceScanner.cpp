// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "SurfaceScanner.h"

SurfaceScanner::SurfaceScanner(Master& master, const Path& root)
	: m_root(root)
	, m_master(master)
	, m_eState(eInitial)
	, m_fProgress(0)
	, m_bSendProgress(true)
{}

void SurfaceScanner::tick(bool& bComplete)
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

void SurfaceScanner::setup()
{
	startRepeater();

	class SkipRoot {};

	Error error;

	m_report.setTaskDescription(m_root);

	try
	{
		bool bDir = m_root.isDir(error);

		if (error)
			throw SkipRoot();

		if (!bDir)
		{
			error.what(L"Specified folder is not a folder");
			error.who(L"SurfaceScanner::setup");
			throw SkipRoot();
		}

		m_absolute.push(m_root);
		m_progressunit.push(Real64(1));
		m_fProgress = 0;
		progressReport(m_root);
		startFolder(error);

		if (error)
		{
			m_absolute.pop();
			throw SkipRoot();
		}

		m_eState = eWorking;
	}
	catch (SkipRoot&)
	{
		skipWarning(m_root, error);
		teardown();
		m_eState = eComplete;
		return;
	}
};

void SurfaceScanner::work()
{
	UTF16 sSubpath;

	bool bItemAvailable = m_subpath_iterator.peek()(sSubpath);

	if (bItemAvailable)
		processSubpath(sSubpath);
	else
		endFolder();
}

void SurfaceScanner::startFolder(Error& error)
{
	HUTF16List subpaths = m_absolute.peek().allsubs(error);

	if (error)
		return;

	FIItem* folder = startFolder(m_absolute.peek(), m_relative);

	m_ancestor.push(folder);
	m_subpaths.push(subpaths);
	m_subpath_iterator.push(*subpaths);
	if (subpaths->size() > 0)
	{
		m_progressunit.push(m_progressunit.peek() / subpaths->size());
	}
	else
	{
		m_fProgress += m_progressunit.peek();
		progressReport(m_absolute.peek());
		m_progressunit.push(0);
	}
}

void SurfaceScanner::processSubpath(const UTF16& sSubpath)
{
	class SkipSubpath {};
	Error error;

	try
	{
		Path subpath = m_absolute.peek().subpath(error, sSubpath);
		if (error)
			throw SkipSubpath();

		bool bIsDir = subpath.isDir(error);
		if (error)
			throw SkipSubpath();

		if (bIsDir)
		{
			m_absolute.push(subpath);
			m_relative.push(sSubpath);
			startFolder(error);
			if (error)
			{
				m_absolute.pop();
				m_relative.pop();
				throw SkipSubpath();
			}
		}
		else
		{
			m_absolute.push(subpath);
			m_relative.push(sSubpath);
			processFile(error);
			m_relative.pop();
			m_absolute.pop();
			if (error)
				throw SkipSubpath();
		}
	}
	catch (SkipSubpath&)
	{
		skipWarning(UTF16(m_absolute.peek()) + L"\\" + sSubpath, error);
		return;
	}
}

void SurfaceScanner::processFile(Error& error)
{
	processFile(error, m_absolute.peek(), m_relative);
	m_fProgress += m_progressunit.peek();
	progressReport(m_absolute.peek());
}

void SurfaceScanner::endFolder()
{
	m_progressunit.pop();
	m_subpath_iterator.pop();
	m_subpaths.pop();
	FIItem* folder = m_ancestor.pop();

	endFolder(folder);

	m_absolute.pop();

	if (m_relative.empty())
	{
		m_eState = eComplete;
		teardown();
		m_report.setProgress(1);
		m_master.SurfaceScanner_progress(m_report);
	}
	else
	{
		m_relative.pop();
	}
}

FIItem* SurfaceScanner::startFolder(const Path& folder, Array<UTF16>& relative)
{
	return m_master.SurfaceScanner_startFolder(folder, relative);
}

void SurfaceScanner::skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage)
{
	m_master.SurfaceScanner_skipWarning(sAbsolutePath, sErrorMessage);
}

void SurfaceScanner::processFile(Error& error, const Path& file, Array<UTF16>& relative)
{
	m_master.SurfaceScanner_processFile(error, file, relative);
}

void SurfaceScanner::progressReport(const Path& path)
{
	if (m_bSendProgress)
	{
		m_bSendProgress = false;

		m_report.setProgress(m_fProgress);
		m_report.setSubtaskDescription(UTF16(path));
		m_master.SurfaceScanner_progress(m_report);
	}
}

void SurfaceScanner::endFolder(FIItem* folder)
{
	m_master.SurfaceScanner_endFolder(folder);
}

void SurfaceScanner::teardown()
{
	m_repeater.stop();
}

void SurfaceScanner::startRepeater()
{
	m_repeater.setTask(buildMethodTask(this, &SurfaceScanner::flagSendProgress));
	m_repeater.start();
}

void SurfaceScanner::stopRepeater()
{
	m_repeater.stop();
}

void SurfaceScanner::flagSendProgress()
{
	m_bSendProgress = true;
}
