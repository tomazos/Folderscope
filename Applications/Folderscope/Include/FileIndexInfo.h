// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class FileIndexInfo
{
public:
	FileIndexInfo();
	FileIndexInfo(UInt32 iVolumeSerialNumber, UInt32 iFileIndexHigh, UInt32 iFileIndexLow);

	bool operator == (const FileIndexInfo& that) const;
	bool operator != (const FileIndexInfo& that) const;
	bool operator < (const FileIndexInfo& that) const;
	bool operator > (const FileIndexInfo& that) const;
	bool operator <= (const FileIndexInfo& that) const;
	bool operator >= (const FileIndexInfo& that) const;

	static SInt64 comparef(const FileIndexInfo& a, const FileIndexInfo& b);

	static UInt64 hashf(const FileIndexInfo& x);

	UTF16 getString();

private:
	bool m_bIsValid;
	UInt32 m_iVolumeSerialNumber;
	UInt32 m_iFileIndexHigh;
	UInt32 m_iFileIndexLow;
};

DeclComparable(FileIndexInfo);
DeclHashable(FileIndexInfo);
