// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/HTTP.h"

#include "Runtime/Log.h"

#include "shellapi.h"

JSON HTTP::rpc(Error& error, Ascii sUrl, const JSON& jRequest)
{
	TCP tcp;

	const Ascii sHttpUrlPrefix("http://");
	UTF16 sErrorMessage;

	if (!sUrl.startsWith(sHttpUrlPrefix))
	{
		error.what(UTF16::format(L"invalid url <%s>", UTF16(sUrl).ptr()));
		error.who(L"HTTP::rpc");
		return JSON();
	};

	int iHostStartIndex = sHttpUrlPrefix.length();

	int iHostEndIndex = iHostStartIndex;
	while (iHostEndIndex < sUrl.length())
	{
		if (sUrl[iHostEndIndex] == '/')
			break;
		
		iHostEndIndex++;
	}
	int iHostLength = iHostEndIndex - iHostStartIndex;
	Ascii sHost = sUrl.substring(iHostStartIndex, iHostLength);

	tcp.open(error, sHost, 80);

	if (error)
	{	
		return JSON();
	}

	static const Chr8* sPostHeaderFormat = "POST %s HTTP/1.0\r\n"
	"Host: %s\r\n"
	"Content-Type: application/json\r\n"
	"Content-Length: %d\r\n"
	"\r\n";

	UTF8 sPostContent = jRequest.generate(error);
	
	if (error)
		return JSON();

	Ascii sPostHeader = Ascii::format(sPostHeaderFormat, sUrl.ptr(), sHost.ptr(), sPostContent.length());

	int iBytesWritten = 0;

	tcp.write(error, sPostHeader.ptr(), sPostHeader.length(), iBytesWritten);

	if (error)
		return JSON();

	if (iBytesWritten != sPostHeader.length())
	{
		logwarn("JSON::post(%s) -> TCP::write(header) -> bytes_written != post_header_length", UTF16(sUrl).ptr());
	}

	iBytesWritten = 0;

	tcp.write(error, sPostContent.ptr(), sPostContent.length(), iBytesWritten);

	if (error)
		return JSON();

	if (iBytesWritten != sPostContent.length())
	{
		logwarn("JSON::post(%s) -> TCP::write(content) -> bytes_written != post_content_length", UTF16(sUrl).ptr());
	}
	
	tcp.end_write(error);

	if (error)
		return JSON();

	int iTotalBytesRead = 0;
	Buffer buf(0);
	while (true)
	{
		buf.resize(iTotalBytesRead + 4096);
		int iBytesRead = 0;
		tcp.read(error, ((Byte*) buf.ptr()) + iTotalBytesRead, 4096, iBytesRead);

		if (error)
			return JSON();

		iTotalBytesRead += iBytesRead;

		if (iBytesRead == 0)
			break;
	}

	tcp.close(error);

	if (error)
		logwarn("JSON::post(%s) -> TCP::close -> %s", UTF16(sUrl).ptr(), sErrorMessage.ptr());

	buf.resize(iTotalBytesRead+1);
	buf.idx<Chr8> (iTotalBytesRead) = '\0';

	int iStartJSON = 0;
	while (iStartJSON < iTotalBytesRead - 5)
	{
		if (  buf.idx<Chr8>(iStartJSON+0) == '\r'
		  &&  buf.idx<Chr8>(iStartJSON+1) == '\n'
		  &&  buf.idx<Chr8>(iStartJSON+2) == '\r'
		  &&  buf.idx<Chr8>(iStartJSON+3) == '\n' )
		{
			iStartJSON += 4;
			break;
		}

		iStartJSON++;
	}

	const Chr8* pJSON = ((Chr8*) buf.ptr()) + iStartJSON;

	int iLengthJSON = iTotalBytesRead - iStartJSON;
	HInputStream hStreamJSON = NewObject(MemoryInputStream, pJSON, iLengthJSON);
	JSON resultJSON = JSON::parse(hStreamJSON);
	return resultJSON;
}
