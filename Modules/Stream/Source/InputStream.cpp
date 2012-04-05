// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/InputStream.h"

#include "Stream/OutputStream.h"

Blob InputStream::slurp(Error& error)
{
	const int iBlockSize = 4096;

	Size iTotalBytes = 0;
	Buffer buf(iBlockSize);

	while (0x00000000 == (iTotalBytes & 0xC0000000))
	{
		Size iBytesRead = read(error, &(buf.idx<Chr8>(iTotalBytes)), iBlockSize);

		if (error)
		{
			buf.resize(iTotalBytes);
			return Blob(buf);
		}

		iTotalBytes += iBytesRead;

		if (iBytesRead != iBlockSize)
		{
			buf.resize(iTotalBytes);
			return Blob(buf);
		}

		buf.resize(iTotalBytes + iBlockSize);
	}

	error.what(L"Overflow");
	error.who(L"InputStream::slurp");
	return Blob();
}

void InputStream::pipe(Error& error, HOutputStream out, Size iBlockSize)
{
	Buffer buf(iBlockSize);
	Size iBytesRead;

	while (true)
	{
		iBytesRead = read(error, buf.ptr(), iBlockSize);

		if (error)
			return;

		if (iBytesRead == 0)
			return;

		out->write(error, buf.ptr(), iBytesRead);

		if (error)
			return;
	}
}
