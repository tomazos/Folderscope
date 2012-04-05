// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

DeclHandleable(OutputStream);

DeclHandleable(InputStream);
class InputStream : public Handleable
{
public:
	virtual Size read(Error&, void* pBuf, Size iNumBytes) = 0;

	Blob slurp(Error&);
	void pipe(Error&, HOutputStream out, Size iBlockSize = 4096);

protected: InputStream() {}
private: InputStream(const InputStream&) {} 
private: InputStream& operator= (const InputStream&) { return *this; } 
public: virtual ~InputStream() {}
};
