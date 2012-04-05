// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

class LogEntryQueue : LogListener
{
public:
	LogEntryQueue()
		: m_pListener(NULL)
		, m_eStatus(eIdle)
	{

	};

	class Listener
	{
	public:
		virtual void requestProcessing() = 0;
	};

	void setListener(Listener* pListener)
	{
		m_pListener = pListener;
	}

	void removeListener(Listener* pListener)
	{
		m_pListener = NULL;
	}
	
	virtual void processLogEntry(HLogEntry hEntry)
	{
		if (m_eStatus == eIdle && m_pListener)
		{
			setStatus(eProcessing);

			requestProcessing();
		}

		m_queue.add(hEntry);
	}

	HLogEntry takeLogEntry()
	{
		HLogEntry hEntry;

		if (!m_queue.remove(hEntry, 0))
			return NULL;

		return hEntry;
	}

	void notifyEmpty()
	{
		setStatus(eIdle);		
	};

	void requestProcessing()
	{
		if (m_pListener)
			m_pListener->requestProcessing();
	};

private:
	enum EStatus
	{
		eProcessing,
		eIdle
	};

	EStatus m_eStatus;

	void setStatus(EStatus eStatus)
	{
		m_eStatus = eStatus;
	}

	BlockQueue<HLogEntry> m_queue;

	Listener* m_pListener;
};
