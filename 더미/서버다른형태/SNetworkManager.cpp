#include "SNetworkManager.h"

HANDLE clientFlag[3];
HANDLE processFlag;

#pragma region [ServerManager]
void SNetworkManager::init()
{
	int retval;
	m_wVersionReq = MAKEWORD(2, 2);
	if (WSAStartup(m_wVersionReq, &m_wsa) != 0)
		err_quit("WSAStartup()");

	processFlag = CreateEvent(NULL, TRUE, FALSE, NULL);
	for (int i = 0; i < 3; i++)
		clientFlag[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listenSock == INVALID_SOCKET)
		err_quit("Create Listen Socket");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(m_listenSock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) 
		err_quit("bind()");

	retval = listen(m_listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR) 
		err_quit("listen()");
}

void SNetworkManager::acceptClient()
{
	int addrlen;
	addrlen = sizeof(m_sockAddr);
	m_sock = accept(m_listenSock, (struct sockaddr*)&m_sockAddr, &addrlen);
	if (m_sock == INVALID_SOCKET)
		err_quit("accept()");

	hThread[m_clientCnt] = CreateThread(NULL, 0, ClientThread, (LPVOID)m_sock, 0, NULL);
	if (hThread[m_clientCnt] == NULL) { closesocket(m_sock); }

	m_clientCnt++;
}

void SNetworkManager::releaseServer()
{
	closesocket(m_listenSock);
	WaitForMultipleObjects(3, hThread, true, INFINITE);
	for (int i = 0; i < 3; i++)
		CloseHandle(hThread[i]);
	WSACleanup();
}

void SNetworkManager::setType(Packet_Type type)
{
	m_pType = type;
}

void SNetworkManager::err_quit(char* msg)
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
#pragma endregion

#pragma region [ClientManager]
void SClientManager::sendFixed()
{
	int retval;
	Packet_Type sceneData = getType();
	retval = send(m_clientSock, (char*)&sceneData, sizeof(Packet_Type), 0);
	if (retval == SOCKET_ERROR)
		err_quit("Send Fixed Error");
}

void SClientManager::sendVariable()
{

}

void SClientManager::Recv()
{
	int retval;
	PositionData clientNextPos;
	retval = recv(m_clientSock, (char*)&clientNextPos, sizeof(PositionData), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
		err_quit("Recv Error");
	printf("¼ö½ÅÇÑ ÁÂÇ¥ : (%d, %d)", clientNextPos.x, clientNextPos.y);
}

void SClientManager::setSocket(SOCKET arg)
{
	m_clientSock = arg;
}

#pragma endregion

#pragma region [Thread]
DWORD WINAPI ProcessThread(LPVOID arg)
{
	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	SClientManager SCM;
	SCM.setSocket((SOCKET)arg);

	while (1)
	{
		SCM.sendFixed();
	}
	SCM.Recv();
	
	return 0;
}
#pragma endregion