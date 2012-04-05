// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/TCP.h"

#include <winsock2.h>
#include <ws2tcpip.h>

TCP::TCP()
	: m_socket(INVALID_SOCKET)
{}

TCP::ErrorList::ErrorList(Error& error)
	: m_error(error)
{}

void TCP::ErrorList::record(const UTF16& s)
{
	m_sErrorMessageList.add(s);
}

TCP::ErrorList::~ErrorList()
{
	if (m_sErrorMessageList.empty())
		return;

	UTF16List::Iterator it(m_sErrorMessageList);
	UTF16 sErrorMessage;
	UTF16 s;
	bool bFirst = true;
	while (it(s))
	{
		if (bFirst)
		{
			bFirst = false;
			sErrorMessage += UTF16::format(L"\"%s\"", s.ptr());
		}
		else
			sErrorMessage += UTF16::format(L", \"%s\"", s.ptr());
	}
	m_error.what(sErrorMessage);
	m_error.who(L"TCP");
}


void TCP::open(Error& error, const Ascii& sHost, UInt16 iPort)
{
	m_socket = INVALID_SOCKET;

	ErrorList errors(error);

	Ascii sPort = Ascii::format("%hu", iPort);

	addrinfo getaddrinfo_hints;
	Memory::zero(&getaddrinfo_hints, sizeof(getaddrinfo_hints));
	getaddrinfo_hints.ai_family = AF_INET;
	getaddrinfo_hints.ai_socktype = SOCK_STREAM;
	getaddrinfo_hints.ai_protocol = IPPROTO_TCP;

	addrinfo* p_getaddrinfo_response_list = NULL;

	int i_getaddrinfo_result =
		getaddrinfo(sHost.ptr(), sPort.ptr(), &getaddrinfo_hints, &p_getaddrinfo_response_list);

	if (i_getaddrinfo_result != 0)
	{
		int i_getaddrinfo_error = WSAGetLastError();
		UTF16 s_getaddrinfo_error = WindowsError::codeToString(i_getaddrinfo_error);
		errors.record(UTF16::format(L"getaddrinfo: %s", s_getaddrinfo_error.ptr()));
		return;
	}

	UTF16 s_socket_error_combined = L"";
	for (addrinfo* p_getaddrinfo_response = p_getaddrinfo_response_list;
		p_getaddrinfo_response != NULL;
		p_getaddrinfo_response = p_getaddrinfo_response->ai_next)
	{
		m_socket = socket
			( p_getaddrinfo_response->ai_family
			, p_getaddrinfo_response->ai_socktype
			, p_getaddrinfo_response->ai_protocol);

		if (m_socket == INVALID_SOCKET)
		{
			int i_socket_error = WSAGetLastError();
			UTF16 s_socket_error = WindowsError::codeToString(i_socket_error);
			errors.record(UTF16::format(L"socket: %s", s_socket_error.ptr()));
			continue;
		}

		int i_connect_result = connect
			( m_socket
			, p_getaddrinfo_response->ai_addr
			, (int) p_getaddrinfo_response->ai_addrlen);

		if (i_connect_result != 0)
		{
			int i_connect_error = WSAGetLastError();
			UTF16 s_connect_error = WindowsError::codeToString(i_connect_error);
			errors.record(UTF16::format(L"connect: %s", s_connect_error.ptr()));

			closesocket(m_socket);
			m_socket = INVALID_SOCKET;

			continue;
		}

		break;
	}

	freeaddrinfo(p_getaddrinfo_response_list);
	
	if (m_socket == INVALID_SOCKET)
	{
		errors.record(L"Invalid socket");
		return;
	}
}

void TCP::write(Error& error, const void* pBuf, Size iRequestNumBytes, Size& iActualNumBytes)
{
	int i_send_result = send(m_socket, (const char*) pBuf, iRequestNumBytes, 0);

	if (i_send_result == SOCKET_ERROR)
	{
		int i_send_error = WSAGetLastError();
		error.what(WindowsError::codeToString(i_send_error));
		error.who(L"send");
		return;
	}
	else
	{
		iActualNumBytes = i_send_result;
		return;
	}
}

void TCP::end_write(Error& error)
{
	int i_shutdown_result = shutdown(m_socket, SD_SEND);

	if (i_shutdown_result != 0)
	{
		int i_shutdown_error = WSAGetLastError();
		error.what(WindowsError::codeToString(i_shutdown_error));
		error.who(L"shutdown(SD_SEND)");
		return;
	}
	
	return;
}

void TCP::read(Error& error, void* pOutBuf, Size iMaxNumBytes, Size& iActualNumBytes)
{
	int i_recv_result = recv(m_socket, (char*) pOutBuf, iMaxNumBytes, 0);

	if (i_recv_result == SOCKET_ERROR)
	{
		int i_recv_error = WSAGetLastError();
		error.what(WindowsError::codeToString(i_recv_error));
		error.who(L"recv");
		return;
	}
	else
	{
		iActualNumBytes = i_recv_result;
		return;
	}
}

void TCP::end_read(Error& error)
{
	int i_shutdown_result = shutdown(m_socket, SD_RECEIVE);

	if (i_shutdown_result != 0)
	{
		int i_shutdown_error = WSAGetLastError();
		error.what(WindowsError::codeToString(i_shutdown_error));
		error.who(L"shutdown(SD_RECEIVE)");
		return;
	}

	return;
}

void TCP::close(Error& error)
{
	if (m_socket == INVALID_SOCKET)
	{
		error.what(L"Invalid socket");
		error.who(L"TCP::close");
		return;
	}

	bool bSuccess = true;

	ErrorList errors(error);

	int i_shutdown_result = shutdown(m_socket, SD_BOTH);

	if (i_shutdown_result != 0)
	{
		int i_shutdown_error = WSAGetLastError();
		UTF16 s_shutdown_error = WindowsError::codeToString(i_shutdown_error);
		errors.record(UTF16::format(L"shutdown(SD_BOTH): %s", s_shutdown_error.ptr()));
	}

	int i_closesocket_result = closesocket(m_socket);

	if (i_closesocket_result != 0)
	{
		int i_closesocket_error = WSAGetLastError();
		UTF16 s_closesocket_error = WindowsError::codeToString(i_closesocket_error);
		errors.record(UTF16::format(L"closesocket: %s", s_closesocket_error.ptr()));
	}

	m_socket = INVALID_SOCKET;

	return;
}

TCP::~TCP()
{
	if (m_socket != INVALID_SOCKET)
	{
		Error error;
		close(error);
		Check(!error);
	}
}
