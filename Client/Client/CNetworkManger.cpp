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

	m_thread = CreateThread(NULL, 0, ServerThread, (LPVOID)this, 0, NULL);
}

int CNetworkManger::SC_Recv_Fixed()
{
	Packet_Type pType{};

	int retval = recv(m_sock, (char*)&pType, sizeof(pType), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_quit(L"패킷 타입 recv()");
		closesocket(m_sock);
	}

	if (pType != Packet_Type::NONE && pType != m_pType)			//	패킷타입이 쓰레기값이 아니고, 현재 씬타입과 다르다면 바꾼다. 여기서만 동기화해주면 좋을 듯 하다.
		m_pType = pType;

	return retval;
}

int CNetworkManger::CS_Send_input(Packet_Type _pType)
{
	int retval{};
	bool bState{};

	switch (_pType)
	{
	case Packet_Type::LOBBY:
		bState = m_fw->getReady();
		retval = send(m_sock, (char*)&bState, sizeof(bool), 0);
		if (retval == SOCKET_ERROR)
			err_quit(L"send()");
		break;

	case Packet_Type::MAIN:
		retval = send(m_sock, (char*)&Reflectors[0].angle, sizeof(float), 0);
		if (retval == SOCKET_ERROR)
			err_quit(L"send()");
		break;

	case Packet_Type::END:
		break;

	case Packet_Type::NONE:
		cout << "패킷 타입 NONE, CS_Send_input()" << endl;
		break;
	}
	
	return retval;
}

int CNetworkManger::SC_Recv_Variable()
{
	int retval = recv(m_sock, (char*)&Reflectors[0].angle, sizeof(float), MSG_WAITALL);			// 각도값 받는다.
	if (retval == SOCKET_ERROR)
	{
		err_quit(L"패킷 타입 recv()");
		closesocket(m_sock);
	}
	return retval;
}

CNetworkManger::CNetworkManger()
{
}

CNetworkManger::CNetworkManger(WGameFramework* FW)
{
	m_chIP = NOW_IP;
	ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = htons(9000);
	m_sockAddr.sin_addr.S_un.S_addr = inet_addr(m_chIP);
	m_iBufferSize = 4'096;
	m_fw = FW;

	init();

}

CNetworkManger::~CNetworkManger()
{
	closesocket(m_sock);
	WSACleanup();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

DWORD WINAPI ServerThread(LPVOID arg)
{
	int retval{};
	CNetworkManger* net = (CNetworkManger*)arg;

	while (true)
	{
		retval = net->SC_Recv_Fixed();						//	패킷타입 변했을시에만 저장, 동기화 최소화
		if (retval == SOCKET_ERROR)
		{
			cout << "recv_fixed()" << endl;
			return -1;
		}

		retval = net->CS_Send_input(net->getFW()->getType());	//	read작업이라 동기화 필요 없음
		if (retval == SOCKET_ERROR)
		{
			cout << "Send_input() Error" << endl;
			return -1;
		}

		retval = net->SC_Recv_Variable();					//	write작업이라 동기화 필요
		if (retval == SOCKET_ERROR)
		{
			cout << "recv_Variable()" << endl;
			return -1;
		}
	}
	return 0;
}