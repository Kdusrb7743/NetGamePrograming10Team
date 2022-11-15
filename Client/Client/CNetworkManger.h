#pragma once
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

class CNetworkManger
{
public:
	void init();

private:
	WGameFramework*			m_fw;
	HANDLE					m_thread;

	char*					m_chIP;
	UINT					m_iBufferSize;

	WSADATA					m_wsa;
	WORD					m_wVersionReq;
	SOCKET					m_sock;
	SOCKADDR_IN				m_sockAddr;

private:
	Packet_Type				m_pType;


public:
	WGameFramework* getFW() {return m_fw;}
	int SC_Recv_Fixed();
	int CS_Send_input(Packet_Type);
	int SC_Recv_Variable();


public:
	CNetworkManger();
	CNetworkManger(WGameFramework*);
	~CNetworkManger();

	void err_quit(WCHAR*);

};

DWORD WINAPI ServerThread(LPVOID arg);

////////////////////////////////////////////////////////////////////////////////////////////

wchar_t* convertCharArrayToLPCWSTR(const char* charArray);

