#pragma once
#pragma comment(lib, "ws2_32")

#include <WinSock2.h>

class CNetworkManger
{
public:
	void init();

	void recv_data(Packet_Type* m_SceneType, bool* m_SceneChange, bool* m_bReady);
	//void recv_variable();

	void Send(bool);
	void Send(PositionData);
	void Send(float);


public:			//	getter
	Packet_Type getType() { return m_pType; }

public:			//	setter
	void setType(Packet_Type pType) { m_pType = pType; }

private:
	char*					m_chIP;
	UINT					m_iBufferSize;

	WSADATA					m_wsa;
	WORD					m_wVersionReq;
	SOCKET					m_sock;
	SOCKADDR_IN				m_sockAddr;

private:
	Packet_Type				m_pType;


public:
	CNetworkManger();
	~CNetworkManger();

	void err_quit(WCHAR*);

};

wchar_t* convertCharArrayToLPCWSTR(const char* charArray);
