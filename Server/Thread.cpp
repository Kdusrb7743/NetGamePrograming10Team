#include "Thread.h"

extern HANDLE clientFlag[PLAYERNUM];
extern HANDLE processFlag;

extern PacketType packetType;
extern clientData client[PLAYERNUM];
extern BallData Ball;


DWORD WINAPI ProcessThread(LPVOID arg)
{
	int retval;
	clock_t start;
	while (true)
	{
		//WaitForMultipleObjects(PLAYERNUM, clientFlag, TRUE, INFINITE);
		switch (packetType)
		{
			case PacketType::NONE:
				printf("packetType Error\n");
				break;
			case PacketType::LOBBY:
			{
				if (CheckPlayerReady())
				{
					packetType = MAIN;
					InitBall();
					start = clock();
				}
				break;
			}
			case PacketType::MAIN:
			{
				clock_t end = clock();
				double time = double(end - start) / CLOCKS_PER_SEC;
				// printf("time : %lf\n", time);
				UpdateBallData(time);
				if (Ball.m_BallSpeed < FLT_MIN)
				{
					start = clock();
				}
				CalculateCollision();
				if (CheckGameOver() == true)
				{
					packetType = END;
				}
				break;
			}
			case PacketType::END:
			{
				break;
			}
			default:
			{
				printf("packetType Error\n");
				break;
			}
		}
		clock_t start = clock();
		//SetEvent(processFlag);
		//ResetEvent(processFlag);
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
		retval = SC_SendFixedData(client_sock);
		if (retval == SOCKET_ERROR)
		{
			std::cout << "SendFixedData Error!" << std::endl;
			return -1;
		}
		retval = CS_RecvData(client_sock, clientPID);
		if (retval == SOCKET_ERROR)
		{
			std::cout << "RecvData Error!" << std::endl;
			return -1;
		}
		//SetEvent(clientFlag[clientPID]);
		//WaitForSingleObject(processFlag, INFINITE);
		//ResetEvent(clientFlag[clientPID]);
		//SC_SendVariableData(client_sock, clientPID);
	}
	return 0;
}