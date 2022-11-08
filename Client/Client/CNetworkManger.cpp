#include "stdafx.h"
#include "CNetworkManger.h"
#include <atlbase.h>

void CNetworkManger::err_quit(WCHAR* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CNetworkManger::init()
{
	m_wVersionReq = MAKEWORD(2,2);
	if (WSAStartup(m_wVersionReq, &m_wsa) != 0)
		err_quit(L"WSAStartup()");

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
		err_quit(L"Socket()");

	int retval = connect(m_sock, (SOCKADDR*)&m_sockAddr, sizeof(m_sockAddr));
	if (retval == SOCKET_ERROR)
		err_quit(L"Connect");
}

void CNetworkManger::recv_fixed()
{
	Packet_Type pType{};

	int retval = recv(m_sock, (char*)&pType, sizeof(pType), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_quit(L"패킷 타입 recv()");
		closesocket(m_sock);
	}

	if(pType == Packet_Type::NONE)
		m_pType = pType;

	setType(pType);
}

void CNetworkManger::recv_variable()
{
}

void CNetworkManger::Send(PositionData pos)
{
	int retval = send(m_sock, (char*)&pos, sizeof(pos), 0);
	if (retval == SOCKET_ERROR)
		err_quit(L"send()");
}

CNetworkManger::CNetworkManger()
{
	m_chIP = NOW_IP;
	ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = htons(9000);
	m_sockAddr.sin_addr.S_un.S_addr = inet_addr(m_chIP);
	m_iBufferSize = 4'096;

}

CNetworkManger::~CNetworkManger()
{
	closesocket(m_sock);
	WSACleanup();
}

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

