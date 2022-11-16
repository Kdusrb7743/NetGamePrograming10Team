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

void CNetworkManger::recv_data(Packet_Type* m_SceneType, bool* m_SceneChange, bool* m_bReady)
{
	Packet_Type pType{};
	SC_MainPacket MainPacket{};
	char clientPID;
	int trash = 0;

	cout << "100\n";

	if (*m_SceneType != m_pType)		// 애니메이션
	{
		*m_SceneChange = true;
		*m_SceneType = getType();
		cout << "현재 Scene 타입: " << *m_SceneType << endl;
	}
	cout << "200\n";

	int retval = recv(m_sock, (char*)&pType, sizeof(pType), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
	{
		err_quit(L"패킷 타입 recv()");
		closesocket(m_sock);
	}

	cout << "300\n";

	if(pType != Packet_Type::NONE)
		m_pType = pType;
	cout << pType << endl;

	//받은 키 입력을 토대로 다음 클라이언트 위치 값을 계산한다.
	switch (pType)
	{
	case Packet_Type::LOBBY:
		cout << "400\n";
									// 서버로부터 PID를 받음
		retval = recv(m_sock, (char*)&clientPID, sizeof(clientPID), MSG_WAITALL);
		if (retval == SOCKET_ERROR)
		{
			err_quit(L"패킷 타입 recv()");
			closesocket(m_sock);
		}

		cout << "로비 패킷 잘 받음.\n";

		Send(*m_bReady);
		break;

	case Packet_Type::MAIN:

		//	클라이언트의 post position을 send()한다.
		//	서버로 부터 승인된 post position을 recv()한다.

		cout << "500\n";

		retval = recv(m_sock, (char*)&MainPacket, sizeof(SC_MainPacket) - sizeof(Packet_Type), MSG_WAITALL);
		Reflectors[0].angle = MainPacket.m_clientPos[0];
		Reflectors[1].angle = MainPacket.m_clientPos[1];
		Reflectors[2].angle = MainPacket.m_clientPos[2];
		//PScore[0] = MainPacket.m_clientScore;


		Send(Reflectors[0].angle);						// 일단 나의 각도 값만 보낸다. MyclientNum

		break;

	case Packet_Type::END:
		break;

	case Packet_Type::NONE:
		cout << "None 타버림.\n";
		//retval = recv(m_sock, (char*)&trash, sizeof(trash), MSG_WAITALL);
		break;
	}

}

void CNetworkManger::Send(bool bReady)
{
	cout << "400\n";
	int retval = send(m_sock, (char*)&bReady, sizeof(bReady), 0);
	if (retval == SOCKET_ERROR)
		err_quit(L"send()");
}

void CNetworkManger::Send(PositionData pos)
{
	int retval = send(m_sock, (char*)&pos, sizeof(pos), 0);
	if (retval == SOCKET_ERROR)
		err_quit(L"send()");
}

void CNetworkManger::Send(float angle)
{
	int retval = send(m_sock, (char*)&angle, sizeof(angle), 0);
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

