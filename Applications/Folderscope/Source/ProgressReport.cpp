// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "ProgressReport.h"

ProgressReport::ProgressReport()
	: m_eState(eInitial)
{}

void ProgressReport::setTaskDescription(const UTF16& sTaskDescription)
{
	m_sTaskDescription = sTaskDescription;
}

void ProgressReport::setSubtaskDescription(const UTF16& sSubtaskDescription)
{
	m_sSubtaskDescription = sSubtaskDescription;
}

void ProgressReport::start()
{
	if (m_eState == eInitial)
	{
		m_fProgress = 0;
		m_tStarted = TimePoint::now();
		m_tCurrent = m_tStarted;
		m_eState = eStarted;
	}
}

void ProgressReport::update(Real64 fProgress)
{
	if (m_eState == eStarted || m_eState == eProgressing)
	{
		m_fProgress = fProgress;
		m_tCurrent = TimePoint::now();
		m_tComplete = m_tStarted + (m_tCurrent - m_tStarted) / m_fProgress;
		m_eState = eProgressing;
	}
}

void ProgressReport::complete()
{
	if (m_eState == eStarted || m_eState == eProgressing)
	{
		m_fProgress = 1;
		m_tCurrent = TimePoint::now();
		m_tComplete = m_tCurrent;
		m_eState = eCompleted;
	}
}

void ProgressReport::setProgress(Real64 fProgress)
{
	if (fProgress <= Real64(0))
		start();
	else if (fProgress >= Real64(1))
		complete();
	else
		update(fProgress);
}

UTF16 ProgressReport::getTaskDescription() const
{
	return m_sTaskDescription;
}

Real64 ProgressReport::getProgress() const
{
	if (m_eState == eInitial || m_eState == eStarted || m_fProgress < Real64(0))
		return 0;
	else if (m_eState == eCompleted || m_fProgress > Real64(1))
		return 1;
	else
		return m_fProgress;
}

UTF16 ProgressReport::getSubtaskDescription() const
{
	return m_sSubtaskDescription;
}

UTF16 ProgressReport::getStartTime() const
{
	if (m_eState == eInitial)
		return L"---";
	else
		return m_tStarted.toString("Y-M-D h:m:s");
}

UTF16 ProgressReport::getCompleteTime() const
{
	if (m_eState == eInitial || m_eState == eStarted)
		return L"---";
	else
		return m_tComplete.toString("Y-M-D h:m:s");
}

UTF16 ProgressReport::getRunningPeriod() const
{
	if (m_eState == eInitial)
		return L"---";
	else
		return (m_tCurrent - m_tStarted).toString();
}

UTF16 ProgressReport::getCompletePeriod() const
{
	if (m_eState == eInitial || m_eState == eStarted)
		return L"---";
	else
		return (m_tComplete - m_tCurrent).toString();
}

UTF16 ProgressReport::getTotalPeriod() const
{
	if (m_eState == eInitial || m_eState == eStarted)
		return L"---";
	else
		return (m_tComplete - m_tStarted).toString();
}
