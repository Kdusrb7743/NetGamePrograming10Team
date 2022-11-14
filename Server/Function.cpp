#include "Function.h"
#include <random>

random_device rnd;
uniform_real_distribution<float> dist(0.f, 1.f);  // ���� [0.0, 1.0)

extern PacketType packetType;
extern clientData client[PLAYERNUM];
extern BallData Ball;


void InitBall()		// �� �ӵ� �� ����(����) �ʱ�ȭ, ���� �� ���� ����
{
	Ball.m_BallPos.x = 0;
	Ball.m_BallPos.y = 0;
	Ball.m_BallAngle = dist(rnd);			// �Ź� ������
	Ball.m_BallSpeed = 1;				// �⺻ ���ǵ�
	Ball.m_BallCount = 3;				// �⺻ �� ����
	Ball.m_BallSpeedx = 0;				// �� ���� �����̱� �� ������ ���� x�̵���
	Ball.m_BallSpeedy = 0;

}

bool BarCollision() {		// �ݻ���(�÷��̾�)�� �浹�� ����
	return false;
}

bool EndCheck() {			// ������ ���������� üũ
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
			retval = 10;				// �̺κ� ���߿� �۾��� �� ���ش�.
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

