#include "Function.h"
#include <random>

random_device rnd;
uniform_real_distribution<float> dist(0.f, 1.f);  // ���� [0.0, 1.0)

extern PacketType packetType;
extern clientData client[PLAYERNUM];
extern BallData Ball;

void InitBall()		// �� �ӵ� �� ����(����) �ʱ�ȭ, ���� �� ���� ����
{
	Ball.m_BallAngle = dist(rnd);			// �Ź� ������
	Ball.m_BallPos.x = cos((2 * PI) * Ball.m_BallAngle);
	Ball.m_BallPos.y = sin((2 * PI) * Ball.m_BallAngle);
	Ball.m_BallSpeed = 0;				// �⺻ ���ǵ�
	Ball.m_BallSpeedx = 0;				// �� ���� �����̱� �� ������ ���� x�̵���
	Ball.m_BallSpeedy = 0;
}

void InitClient()
{
	client[0].m_clientAngle = 0.25f;
	client[1].m_clientAngle = 0.583f;
	client[2].m_clientAngle = 0.915f;
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
	//printf("BallPos (X: %f, Y: %f)\n", Ball.m_BallPos.x, Ball.m_BallPos.y);
}

void CalculateCollision()
{
	CheckBarCollision();
	CheckBallCollision();
}

void SetBallPosition()
{
	Ball.m_BallPos.x += Ball.m_BallSpeedx;
	Ball.m_BallPos.y += Ball.m_BallSpeedy;
	if (DistanceOvercmp(Ball.m_BallPos.x, Ball.m_BallPos.y, 525))
	{
		//printf("BallPos (X: %f, Y: %f)\n", Ball.m_BallPos.x, Ball.m_BallPos.y);
		//printf("BallAngle : %f\n", Ball.m_BallAngle);

		//Ball.m_BallCount--;
		InitBall();
	}
}

void OrbSpeed()
{
	Ball.m_BallSpeedx = Ball.m_BallSpeed * cos(2 * PI * (Ball.m_BallAngle)) * 5;
	Ball.m_BallSpeedy = Ball.m_BallSpeed * sin(2 * PI * (Ball.m_BallAngle)) * 5;

	Ball.m_BallAngle = (atan2(-Ball.m_BallPos.y, -Ball.m_BallPos.x) / (2 * PI)) + 0.5;
	printf("BallAngle : %f\n", Ball.m_BallAngle);
}

void SpeedCaculate(double time)
{
	if (time < FLT_MIN)
	{
		Ball.m_BallSpeed = 0.01f;
	}
	else
	{
		Ball.m_BallSpeed = time * 0.01f;
	}
	// printf("Ball Speed = %f\n", Ball.m_BallSpeed);
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

void CheckBallCollision()
{
	
	for (int i = 0; i < 3; ++i)
	{
		if (Intersect(client[i], Ball))		//	�гΰ� ���� �浹 �Ѵٸ�
		{
			//	Ball.m_BallAngle =		//	�浹 �� �ޱ� �� ����
			client[i].m_clientScore += 10;
			return;
		}
		else if (DistanceOvercmp(Ball.m_BallPos.x, Ball.m_BallPos.y, 525))	//	���� ȭ�� ������ �����ٸ�
			--Ball.m_BallCount;
	}
}

void Collision_Ball()
{
	DirectX::BoundingOrientedBox ball;
	ball.Center.x = Ball.m_BallPos.x, ball.Center.y = Ball.m_BallPos.y;
	ball.Extents.x = 15, ball.Extents.y = 15;

	for (int i = 0; i < 3; ++i)
	{
		DirectX::BoundingOrientedBox player;
		player.Center.x = client[i].m_clientNextPos.x, player.Center.y = client[i].m_clientNextPos.y;
		player.Extents.x;
		if (ball.Intersects(player))
			//
			return;
	}
	
}

void CollisionDetect(struct Power_Orb* Orb)								// ���� - ���� �� �浹 �ߴ���
{
	
}

bool DistanceOvercmp(float x, float y, float dis)
{
	return ((x * x) + (y * y) > dis * dis);
}

float AnglePosition(float x, float y)
{
	return atan2(-y, -x) / (PI * 2) + 0.5;
}

int SC_SendVariableData(SOCKET client_sock, int clientPID)
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
		SC_LobbyPacket LobbyPacket;
		LobbyPacket.m_clientPID = (char)clientPID;
		retval = send(client_sock, (char*)&LobbyPacket, sizeof(SC_LobbyPacket), 0);
		break;
	}
	case PacketType::MAIN:
	{
		SC_MainPacket MainPacket;
		MainPacket.m_ballPos = Ball.m_BallPos;
		MainPacket.m_clientPos[0] = client[0].m_clientAngle;
		MainPacket.m_clientPos[1] = client[1].m_clientAngle;
		MainPacket.m_clientPos[2] = client[2].m_clientAngle;
		MainPacket.m_clientScore = client[clientPID].m_clientScore;							//�ڱ� ���ھ�

		retval = send(client_sock, (char*)&MainPacket, sizeof(SC_MainPacket), 0);		// ������ ������
		break;
	}
	case PacketType::END:
	{
		SC_EndPacket EndPacket;
		//EndPacket.m_clientScore[0] = 0;
		retval = 10;
		break;
	}
	}
	return retval;
}



int CS_RecvData(SOCKET client_sock, int clientPID)
{
	int retval;
	int Trash;
	switch (packetType)
	{
		case PacketType::NONE:
		{
			printf("Packet Type Error!\n");
			retval = recv(client_sock, (char*)&Trash, sizeof(Trash), MSG_WAITALL);
			break;
			//return -1;
		}
		case PacketType::LOBBY:
		{
			retval = recv(client_sock, (char*)&(client[clientPID].m_clientReady), sizeof(bool), MSG_WAITALL);
			break;
		}
		case PacketType::MAIN:
		{
			//retval = 10;				// �̺κ� ���߿� �۾��� �� ���ش�.
			// retval = recv(client_sock, (char*)&client[clientPID].m_clientNextPos, sizeof(CS_MainPacket), MSG_WAITALL);
			retval = recv(client_sock, (char*)&client[clientPID].m_clientAngle, sizeof(float), MSG_WAITALL);
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

bool Intersect(clientData _c, BallData _b)
{
	//	�� �гε��� �ޱ� �ִ� �ּҰ� �ʿ�
	if ( AngleDetect(_b.m_BallPos.x, _b.m_BallPos.y, _b.m_BallAngle) && _b.m_BallPos.x < 500)
		//	���� �ʿ� (�г� �ޱ� �ּ� ���� ũ�� �ִ뺸�� �����鼭 ���� ��ġ�� �гΰ� �´��� �� )
		return true;
	else
		return false;

	
}

bool AngleDetect(float x, float y, float Angle)						// �浹�� - �гΰ��� ���� ����
{
	Angle = AngleOverflow(AnglePosition(x, y) - Angle);
	return (Angle < 0.13) || (0.87 < Angle);
}

float AngleOverflow(float Angle)									// ���� 360�ʰ��� �ٽ� 0���� ������.
{
	if (Angle >= 1)
		Angle--;
	else if (Angle < 0)
		Angle++;
	return Angle;
}