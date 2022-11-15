#include "Function.h"
#include <random>

random_device rnd;
uniform_real_distribution<float> dist(0.f, 1.f);  // 범위 [0.0, 1.0)

extern PacketType packetType;
extern clientData client[PLAYERNUM];
extern BallData Ball;


void InitBall()		// 공 속도 및 각도(벡터) 초기화, 남은 공 개수 감소
{
	Ball.m_BallAngle = dist(rnd);			// 매번 랜덤값
	Ball.m_BallPos.x = cos((2 * PI) * Ball.m_BallAngle);
	Ball.m_BallPos.y = sin((2 * PI) * Ball.m_BallAngle);
	Ball.m_BallSpeed = 0;				// 기본 스피드
	// Ball.m_BallCount = 3;				// 기본 공 개수
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
	Ball.m_BallPos.x += Ball.m_BallSpeedx * cos((2 * PI) * Ball.m_BallAngle) * 5;
	Ball.m_BallPos.y += Ball.m_BallSpeedy * sin((2 * PI) * Ball.m_BallAngle) * 5;
	if (DistanceOvercmp(Ball.m_BallPos.x, Ball.m_BallPos.y, 525))
	{
		Ball.m_BallCount--;
		InitBall();
	}
}

void OrbSpeed()
{
	Ball.m_BallSpeedx = Ball.m_BallSpeed;
	Ball.m_BallSpeedy = Ball.m_BallSpeed;

	Ball.m_BallAngle = (atan2(-Ball.m_BallPos.y, -Ball.m_BallPos.x) / (2 * PI)) + 0.5;
	printf("BallAngle : %f\n", atan2(-Ball.m_BallPos.y, -Ball.m_BallPos.x));
}

void SpeedCaculate(double time)
{
	Ball.m_BallSpeed = time * 0.01f;
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
	
}

void CollisionDetect(struct Power_Orb* Orb)								// 서버 - 메인 공 충돌 했는지
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

