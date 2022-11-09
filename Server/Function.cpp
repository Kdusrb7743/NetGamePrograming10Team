#include "Function.h"
#include <random>

random_device rnd;
uniform_real_distribution<float> dist(0.f, 1.f);  // 범위 [0.0, 1.0)

extern PacketType packetType;
extern clientData client[PLAYERNUM];
extern BallData Ball;


void InitBall()		// 공 속도 및 각도(벡터) 초기화, 남은 공 개수 감소
{
	Ball.m_BallPos.x = 0;
	Ball.m_BallPos.y = 0;
	Ball.m_BallAngle = dist(rnd);			// 매번 랜덤값
	Ball.m_BallSpeed = 1;				// 기본 스피드
	Ball.m_BallCount = 3;				// 기본 공 개수
	Ball.m_BallSpeedx = 0;				// 매 공이 움직이기 전 각도에 따른 x이동량
	Ball.m_BallSpeedy = 0;

}

bool BarCollision() {		// 반사판(플레이어)간 충돌을 감지
	return false;
}

bool EndCheck() {			// 게임이 끝났는지를 체크
	if (Ball.m_BallCount == 0) return true;
	else return false;
}

void UpdateBallData()
{

}

void CalculateCollision()
{

}

struct BallData* OrbPosition(struct BallData* Orb)
{

	Orb->m_BallPos.x += Orb->m_BallSpeedx;
	Orb->m_BallPos.y += Orb->m_BallSpeedy;

	return Orb;
}

bool CheckPlayerReady()
{
	int readyPlayerCNT = 0;
	for (int i = 0; i < PLAYERNUM; i++)
	{
		if (client[i].m_clientReady)
		{
			readyPlayerCNT++;
		}
	}
	if (readyPlayerCNT == PLAYERNUM)
		return true;

	return false;
}

bool CheckGameOver()
{
	if (Ball.m_BallCount == 0)
	{
		return true;
	}
	return false;
}

int SC_SendFixedData(SOCKET client_sock)
{
	int retval;
	retval = send(client_sock, (char*)&packetType, sizeof(PacketType), 0);
	return retval;
}

int CS_RecvData(SOCKET client_sock, int clientPID)
{
	int retval;
	switch (packetType)
	{
		case PacketType::NONE:
		{
			printf("Packet Type Error!\n");
			return -1;
		}
		case PacketType::LOBBY:
		{
			retval = recv(client_sock, (char*)&(client[clientPID].m_clientReady), sizeof(CS_LobbyPacket), MSG_WAITALL);
			break;
		}
		case PacketType::MAIN:
		{
			retval = 10;
			// retval = recv(client_sock, (char*)&client[clientPID].m_clientNextPos, sizeof(CS_MainPacket), MSG_WAITALL);
			break;
		}
		case PacketType::END:
		{
			retval = 10;
			break;
		}
	}
	return retval;
}

