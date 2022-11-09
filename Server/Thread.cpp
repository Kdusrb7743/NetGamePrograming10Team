#include "Thread.h"

DWORD WINAPI ProcessThread(LPVOID arg)
{
	int retval;
	while (true)
	{
		WaitForMultipleObjects(PLAYERNUM, clientFlag, TRUE, INFINITE);
		printf("±×³É ¶Õ¸²\n");
		switch (packetType)
		{
			case NONE:
				printf("packetType Error\n");
				break;
			case LOBBY:
			{
				if (CheckPlayerReady(client))
				{
					packetType = MAIN;
					//SetEvent(processFlag);
				}
				break;
			}
			case MAIN:
			{
				printf("Enter the Main Scene\n");
				UpdateBallData();
				CalculateCollision();
				if (CheckGameOver() == true)
				{
					packetType = END;
				}
				SetEvent(processFlag);
				ResetEvent(processFlag);
				break;
			}
			case END:
				break;
			default: 
			{
				printf("packetType Error\n");
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	static int clientNum = 0;
	int clientPID = clientNum++;
	client[clientPID].m_clientReady = true;
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	while (1)
	{
		retval = send(client_sock, (char*)&packetType, sizeof(PacketType), 0);
		retval = WaitForSingleObject(processFlag, INFINITE);
	}
	
	//PositionData clientNextPos;
	//retval = recv(client_sock, (char*)&clientNextPos, sizeof(PositionData), MSG_WAITALL);

	return 0;
}