#include "Thread.h"

PacketType packetType = LOBBY;

DWORD WINAPI ProcessThread(LPVOID arg)
{
	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	static int clientNum = 0;
	int clientPID = clientNum++;
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	while (1)
	{
		printf("%d\n", packetType);
		retval = send(client_sock, (char*)&packetType, sizeof(PacketType), 0);
	}
	
	//PositionData clientNextPos;
	//retval = recv(client_sock, (char*)&clientNextPos, sizeof(PositionData), MSG_WAITALL);

	return 0;
}