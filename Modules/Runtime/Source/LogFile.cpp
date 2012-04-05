// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/LogFile.h"

LogFile::LogFile(Error& error, const UTF16& sLogFilePath)
	: m_sLogFilePath(sLogFilePath)
{
	Path logFilePath(error, m_sLogFilePath);

	if (error)
		return;

	m_logFile = NewObject(FileOutputStream, error, logFilePath, FileOutputStream::eAppend);

	if (error)
	{
		m_logFile.setNull();
		return;
	}

	loginfo("Attached log file \"%s\"", m_sLogFilePath.ptr());
}

LogFile::~LogFile()
{
	loginfo("Detaching log file \"%s\"", m_sLogFilePath.ptr());
}

void LogFile::processLogEntry(HLogEntry hEntry)
{
	if (hEntry.isSet() && m_logFile.isSet() && !m_error)
	{
		UTF16 sMessage = hEntry->getFullMessage();

		m_logFile->twrite(m_error,UTF8(sMessage));

		if (m_error)
		{		
			logerr("Unable to write to logfile \"%s\"", m_sLogFilePath.ptr());
			return;
		}

		m_logFile->twrite(m_error, UTF8("\r\n"));

		if (m_error)
		{		
			logerr("Unable to write to logfile \"%s\"", m_sLogFilePath.ptr());
			return;
		}
	}
}
