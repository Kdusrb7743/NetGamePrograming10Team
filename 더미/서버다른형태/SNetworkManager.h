#pragma once
#include "Global.h"

DWORD WINAPI ProcessThread(LPVOID arg);
DWORD WINAPI ClientThread(LPVOID arg);

class SNetworkManager
{
public:
	void		init();
	void		acceptClient();
	void		releaseServer();

	//HANDLE clientFlag[3];
	//HANDLE processFlag;

public:			//	getter
	Packet_Type getType() { return m_pType; }

public:			//	setter
	void setType(Packet_Type type);

private:
	UINT					m_iBufferSize;

	WSADATA					m_wsa;
	WORD					m_wVersionReq;

	SOCKET					m_sock;
	struct sockaddr_in		m_sockAddr;

	SOCKET					m_listenSock;
	SOCKADDR_IN				m_listenSockAddr;


	HANDLE					hThread[3];
private:
	Packet_Type				m_pType = LOBBY;


public:
	SNetworkManager() {};
	~SNetworkManager() {};

	void err_quit(char*);

	int						m_clientCnt = 0;
};

class SClientManager : public SNetworkManager
{
public:
	void sendFixed();
	void sendVariable();
	void Recv();
	void setSocket(SOCKET arg);

private:
	SOCKET					m_clientSock;
	SOCKADDR_IN				m_clientSockAddr;

	int						m_clientPID;

public:
	SClientManager() { m_clientPID = m_clientCnt;}
	~SClientManager() {};

};