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
	Ball.m_BallSpeed = 0;				// �⺻ ���ǵ�
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

void UpdateBallData(double time)
{
	SpeedCaculate(time);
	OrbSpeed();
	SetBallPosition();
	printf("BallPos (X: %f, Y: %f)\n", Ball.m_BallPos.x, Ball.m_BallPos.y);
}

void CalculateCollision()
{
	CheckBarCollision();
	//CheckBallCollision();
}

void SetBallPosition()
{
	Ball.m_BallPos.x += Ball.m_BallSpeedx;
	Ball.m_BallPos.y += Ball.m_BallSpeedy;
	Ball.m_BallAngle = atan2f(Ball.m_BallPos.x - 1.f, Ball.m_BallPos.y);
	printf("BallAngle : %f\n", Ball.m_BallAngle);
}

void OrbSpeed()
{
	Ball.m_BallSpeedx = Ball.m_BallSpeed * cos(2 * PI * Ball.m_BallAngle) * 5;
	Ball.m_BallSpeedy = Ball.m_BallSpeed * sin(2 * PI * Ball.m_BallAngle) * 5;
}

void SpeedCaculate(double time)
{
	Ball.m_BallSpeed = time;
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

void CheckBarCollision()
{

}

//void CheckBallCollision()
//{
//	if (DistanceOvercmp(Ball.m_BallPos.x, Ball.m_BallPos.y, 500))
//	{
//		if (!DistanceOvercmp(Ball.m_BallPos.x, Ball.m_BallPos.y, 525))
//		{
//
//		}
//		if (((Orb->next->major == false && Orb->next->type == 0) || Orb->next->effect == 1) 
//			&& Distancecmp(Orb->next->x + 0, Orb->next->y + 0, 525))
//		{
//			ReflectOrb(Orb->next, AnglePosition(Orb->next->x, Orb->next->y));
//			if (Orb->next->effect == 1) Orb->next->effect = 0;
//		}
//		else
//		{
//			if (Orb->next->major)
//			{
//				ReactorEffect = 6;
//				if (Orbcount > 0) ReactorEffect = 6;
//				else Orbcount = -1;
//			}
//			OrbRemove(Orb->next, Orb);
//		}
//	}
//	CollisionDetect(Orb->next);
//}

bool DistanceOvercmp(float x, float y, float dis)
{
	return ((x * x) + (y * y) > dis * dis);
}

float AnglePosition(float x, float y)
{
	return atan2(-y, -x) / (PI * 2) + 0.5;
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

