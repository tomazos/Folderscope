// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "Stream/InputStream.h"
#include "Stream/OutputStream.h"
#include "Stream/PathWalker.h"

class Path
{
public:
	static Path getCurrentDir(Error&);
	static void setCurrentDir(Error&, const Path&);

	static Path createTempDir(Error&, const UTF16& sPrefix);
	static Path getAppDataFolder(Error&);

	Path();
	explicit Path(Error& error, const UTF16& sPath);
	operator UTF16() const;
	const Chr16* ptr() const;

	UTF16 basename() const;

	static SInt64 comparef(const Path& a, const Path& b);
	bool startsWith(const UTF16&);
	bool endsWith(const UTF16&);
	bool contains(const UTF16&);

	bool operator== (const Path& path) const;
	bool operator!= (const Path& path) const;
	bool operator< (const Path& path) const;
	bool operator> (const Path& path) const;
	bool operator<= (const Path& path) const;
	bool operator>= (const Path& path) const;

	bool isValid() const;
	bool exists(Error&) const;
	Path parent(Error&) const;

	bool isFile(Error&) const;
	bool isDir(Error&) const;
	bool isRootDir(Error&) const;

	bool walk(PathWalker&) const;

	// read dir
	bool isEmptyDir(Error& error) const;
	Path subpath(Error&, const UTF16& sRelativePath) const;
	HUTF16List allsubs(Error&) const;

	// write dir
	void createDir(Error&) const;
	void vivDir(Error&) const;
	void vivDirForce(Error&) const;
	void deleteDirEmpty(Error&) const;
	void deleteDirForce(Error&) const;

	// read file
	UInt64 fileSize(Error&) const;
	void readFile(Error&, HInputStream&) const;
	void slurpFile(Error&, Blob&) const;

	// write file
	void createFile(Error&, HOutputStream&) const;
	void overwriteFile(Error&, HOutputStream&) const;
	void appendFile(Error&, HOutputStream&) const;
	void deleteFile(Error&) const;

private:
	void parsePath(Error& error, const UTF16& sRawPath);

	bool invalid_(Error& error) const;

	bool m_bValid;
	UTF16 m_sPath;
};

DeclComparable(Path);
