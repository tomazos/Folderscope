// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/LogEntry.h"

LogEntry::LogEntry(ELogLevel eLevel, const UTF16& sMessage)
	: m_eLevel(eLevel)
	, m_sMessage(sMessage)
	, m_when(TimePoint::now())
{

}

LogEntry::ELogLevel LogEntry::getLevel()
{
	return m_eLevel;
}

UTF16 LogEntry::getMessage()
{
	return m_sMessage;
}

TimePoint LogEntry::getWhen()
{
	return m_when;
}

UTF16 LogEntry::getFullMessage()
{
	UTF16 sLevel;
	switch (m_eLevel)
	{
	case eInfo: sLevel = L""; break;
	case eWarning: sLevel = L"WARNING: "; break;
	case eError: sLevel = L"ERROR: "; break;
	}

	return UTF16::format(L"%s: %s%s", UTF16(m_when.toString()).ptr(), sLevel.ptr(), m_sMessage.ptr());
}
