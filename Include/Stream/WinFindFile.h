// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class WinFindFile;

class WinFindData
{
public:
	WinFindData();

	UTF16 getName();
	UInt64 getSize();

	TimePoint getCreationTime();
	TimePoint getLastAccessTime();
	TimePoint getLastWriteTime();

	bool is_archive();
	bool is_compressed();
	bool is_device();
	bool is_directory();
	bool is_encryptred();
	bool is_hidden();
	bool is_normal();
	bool is_not_context_indexed();
	bool is_offline();
	bool is_readonly();
	bool is_reparse_point();
	bool is_sparse_file();
	bool is_system();
	bool is_temporary();
	bool is_virtual();

private: 
	friend class WinFindFile;
	WIN32_FIND_DATA m_data;
};

class WinFindFile : public Handleable
{
public:
	WinFindFile(const UTF16& sPath);

	bool operator() (Error& error, WinFindData&);

	~WinFindFile();

private:
	void close();

	UTF16 m_sPath;
	HANDLE m_h;
};
