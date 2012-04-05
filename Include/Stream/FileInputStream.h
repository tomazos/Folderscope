// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Stream/Path.h"
#include "Stream/InputStream.h"

class FileInputStream : public InputStream
{
public:
	FileInputStream(Error&, const Path& path);
	virtual ~FileInputStream();

	virtual Size read(Error&, void* pBuf, Size iNumBytes);

	void getFileIndexInfo(Error&, UInt32& iVolumeIndex, UInt32& iFileIndexHi, UInt32& iFileIndexLo);

private:

	WindowsHandle m_file;

private: FileInputStream& operator=(const FileInputStream&) {};
};
