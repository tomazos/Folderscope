// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class ProgressReport
{
public:
	ProgressReport();

	void setTaskDescription(const UTF16& sProjectDescription);
	void setProgress(Real64 fProgress);
	void setSubtaskDescription(const UTF16& sTaskDescription);

	UTF16 getTaskDescription() const;
	Real64 getProgress() const;
	UTF16 getSubtaskDescription() const;
	UTF16 getStartTime() const;
	UTF16 getCompleteTime() const;
	UTF16 getRunningPeriod() const;
	UTF16 getCompletePeriod() const;
	UTF16 getTotalPeriod() const;

private:

	void start();
	void update(Real64 fProgress);
	void complete();

	enum EState
	{
		eInitial,
		eStarted,
		eProgressing,
		eCompleted
	};

	EState m_eState;

	UTF16 m_sTaskDescription;
	UTF16 m_sSubtaskDescription;
	TimePoint m_tStarted;
	TimePoint m_tCurrent;
	TimePoint m_tComplete;
	Real64 m_fProgress;
};
