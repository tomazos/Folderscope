// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

typedef UINT_PTR        SOCKET;

DeclHandleable(Handleable);
class TCP : public Handleable
{
public:
	TCP();

	void open(Error&, const Ascii& sHost, UInt16 iPort);

	void write(Error&, const void* pBuf, Size iRequestNumBytes,
		Size& iActualNumBytes);

	void end_write(Error&);

	void read(Error&, void* pOutBuf, Size iMaxNumBytes,
		Size& iActualNumBytes);

	void end_read(Error&);

	void close(Error&);

	virtual ~TCP();

private:
	class ErrorList
	{
	public:
		ErrorList(Error& error);
		void record(const UTF16& s);
		~ErrorList();

	private:
		UTF16List m_sErrorMessageList;
		Error& m_error;
	};

	SOCKET m_socket;

	TCP(const TCP&) {};
};
