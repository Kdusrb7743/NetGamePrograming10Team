#include "Thread.h"

extern HANDLE clientFlag[PLAYERNUM];
extern HANDLE processFlag;

extern PacketType packetType;

extern clientData client[PLAYERNUM];
extern BallData Ball;
clock_t start;

DWORD WINAPI ProcessThread(LPVOID arg)
{
	int retval;
	while (true)
	{
		retval = WaitForMultipleObjects(PLAYERNUM, clientFlag, TRUE, 50);
		if (retval == 0)
		{
			switch (packetType)
			{
				case PacketType::NONE:
					printf("packetType Error\n");
					break;
				case PacketType::LOBBY:
				{
					if (ReadyCheck())
					{
						ChangePacket(MAIN);
						InitBall();
						InitClient();
						start = clock();
					}
					break;
				}
				case PacketType::MAIN:
				{
					clock_t end = clock();
					double time = double(end - start) / CLOCKS_PER_SEC;
					UpdateBallData(time);
					CalculateCollision();
					if (EndCheck())
					{
						ChangePacket(END);
						Ball.m_BallCount = 3;
					}
					break;
				}
				case PacketType::END:
				{
					ChangetoLobby();
					break;
				}
				default:
				{
					printf("packetType Error\n");
					break;
				}
			}
		}
		SetEvent(processFlag);
		ResetEvent(processFlag);
	}
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
		retval = SC_SendData(client_sock, clientPID);
		if (retval == SOCKET_ERROR)
		{
			std::cout << "SendVariableData Error!" << std::endl;
			return -1;
		}
		retval = CS_RecvData(client_sock, clientPID);
		if (retval == SOCKET_ERROR)
		{
			std::cout << "RecvData Error!" << std::endl;
			return -1;
		}
		SetEvent(clientFlag[clientPID]);
		WaitForSingleObject(processFlag, INFINITE);
		ResetEvent(clientFlag[clientPID]);
	}
	return 0;
}