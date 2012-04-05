// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Stream/Path.h"
#include "Stream/OutputStream.h"

class FileOutputStream : public OutputStream
{
public:
	enum EOpenType
	{
		eCreate,
		eOverwrite,
		eAppend
	};

	FileOutputStream(Error&, const Path& path, EOpenType);
	virtual ~FileOutputStream();

	virtual void write(Error&, const void* pBuf, Size iNumBytes);

private:

	WindowsHandle m_file;

private: FileOutputStream& operator=(const FileOutputStream&) {};
};
