#include "Function.h"
#include <random>

random_device rnd;
uniform_real_distribution<float> dist(0.f, 1.f);  // 범위 [0.0, 1.0)

extern PacketType packetType;
extern clientData client[PLAYERNUM];
extern BallData Ball;
extern clock_t start;

void InitBall()		// 공 속도 및 각도(벡터) 초기화, 남은 공 개수 감소
{
	Ball.m_BallAngle = dist(rnd);			// 매번 랜덤값
	Ball.m_BallPos.x = 0;
	Ball.m_BallPos.y = 0;
	Ball.m_BallSpeed = 1.f;					// 기본 스피드
	Ball.m_BallSpeedx = Ball.m_BallSpeed * cos((2 * PI) * Ball.m_BallAngle) * 5;
	Ball.m_BallSpeedy = Ball.m_BallSpeed * sin((2 * PI) * Ball.m_BallAngle) * 5;
	start = clock();
}

void InitClient()
{
	client[0].m_clientAngle = 0.25f;
	client[1].m_clientAngle = 0.583f;
	client[2].m_clientAngle = 0.915f;
	client[0].m_clientScore = 0;
	client[1].m_clientScore = 0;
	client[2].m_clientScore = 0;
}


void BarCollision()
{
	for (int i = 0; i < PLAYERNUM; i++)
	{
		for (int j = i + 1; j < PLAYERNUM; j++)
		{
			if (fabs(client[i].m_clientNextAngle - client[j].m_clientNextAngle) < 0.16 ||
				fabs(client[i].m_clientNextAngle - client[j].m_clientNextAngle) > 0.84)
			{
				client[i].m_clientNextAngle = client[i].m_clientAngle;
				client[j].m_clientNextAngle = client[j].m_clientAngle;
			}
		}
	}

	for (int i = 0; i < PLAYERNUM; i++)
	{
		client[i].m_clientAngle = client[i].m_clientNextAngle;
	}
}

void UpdateBallData(double time)
{
	SpeedCaculate(time);
	SetBallPosition();
}

void CalculateCollision()
{
	BarCollision();

	if (DistanceOvercmp(Ball.m_BallPos.x, Ball.m_BallPos.y, 510))	//	공이 화면 밖으로 나간다면
	{
		InitBall();
		--Ball.m_BallCount;
	}
	for (int i = 0; i < 3; i++) {
		if (DistanceOvercmp(Ball.m_BallPos.x, Ball.m_BallPos.y, 375 - 110) &&
			AngleDetect(Ball.m_BallPos.x, Ball.m_BallPos.y, client[i].m_clientAngle) &&
			Distancecmp(Ball.m_BallPos.x, Ball.m_BallPos.y, 375) &&
			client[i].m_ReflectDelayTime == 0 &&
			DistanceDetect(Ball.m_BallPos.x, Ball.m_BallPos.y, AnglePosition(Ball.m_BallPos.x, Ball.m_BallPos.y) - client[i].m_clientAngle, 375, 375))
		{
			ReflectOrb(client[i].m_clientAngle);
			Ball.m_BallPos.x += Ball.m_BallSpeedx;
			Ball.m_BallPos.y += Ball.m_BallSpeedy;
			client[i].m_clientScore += 10;
			client[i].m_ReflectDelayTime += 15;
			break;
		}
	}
	for (int i = 0; i < 3; i++) {
		if (client[i].m_ReflectDelayTime > 0) {
			client[i].m_ReflectDelayTime--;
		}
	}
}

bool AngleDetect(float x, float y, float Angle)						// 충돌시 - 패널각도 감지 계산용
{
	Angle = AngleOverflow(AnglePosition(x, y) - Angle);
	return (Angle < 0.13) || (0.87 < Angle);
}

float AngleOverflow(float Angle)									// 각도 360초과시 다시 0으로 돌린다.
{
	if (Angle >= 1)
		Angle--;
	else if (Angle < 0)
		Angle++;
	return Angle;
}

bool Distancecmp(float x, float y, float dis)						// 거리가 레일 안쪽이면 트루
{
	return ((x * x) + (y * y) < dis * dis);
}

bool DistanceDetect(float x, float y, float Angle, float Distance, float Size)	// 충돌시 - 거리감지 계산용
{
	return (x * x + y * y > (Distance * Distance - Size * Size * 0.25) / (cos(Angle * 2 * PI) * cos(Angle * 2 * PI)));
}

void ReflectOrb(float Angle)				// 서버 - 반사된 공의 각도 변경
{
	if (ObtuseDetect(AngleOverflow(Ball.m_BallAngle - Angle)))
	{
		if (AngleOverflow(Ball.m_BallAngle - Angle) < 0.5) Ball.m_BallAngle = AngleOverflow(Ball.m_BallAngle + 0.25);
		else Ball.m_BallAngle = AngleOverflow(Ball.m_BallAngle - 0.25);
	}
	else Ball.m_BallAngle = Reflect(Ball.m_BallAngle, Angle);
	OrbSpeed();

}

bool ObtuseDetect(float Angle)										// 둔각으로 충돌시
{
	AngleOverflow(Angle);
	return ((0.125 < Angle && Angle <= 0.25) || (0.75 <= Angle && Angle < 0.875));
}

float Reflect(float Angle, float Reflector)							// 반사된 각도를 반환(튕길 때 공 각도에 적용)
{
	Angle = 0.5 - Angle + Reflector * 2;
	return Angle;
}

void SetBallPosition()
{
	Ball.m_BallPos.x += Ball.m_BallSpeedx;
	Ball.m_BallPos.y += Ball.m_BallSpeedy;
}

void OrbSpeed()
{
	Ball.m_BallSpeedx = Ball.m_BallSpeed * cos((2 * PI) * Ball.m_BallAngle) * 5;
	Ball.m_BallSpeedy = Ball.m_BallSpeed * sin((2 * PI) * Ball.m_BallAngle) * 5;
}

void SpeedCaculate(double time)
{
	Ball.m_BallSpeed = 1.f + 0.2f * (int)time;
}

bool CheckPlayerReady()
{
	int readyPlayerCNT = 0;
	for (int i = 0; i < PLAYERNUM; i++)
	{
		if (client[i].m_clientReady == true)
		{
			readyPlayerCNT++;
		}
	}
	if (readyPlayerCNT == PLAYERNUM)
	{
		readyPlayerCNT = 0;
		return true;
	}

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

	switch (client[clientPID].m_packetType)
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
		MainPacket.m_clientScore = client[clientPID].m_clientScore;							//자기 스코어
		MainPacket.m_ballcount = Ball.m_BallCount;

		retval = send(client_sock, (char*)&MainPacket, sizeof(SC_MainPacket), 0);			// 각도값 보내기
		break;
	}
	case PacketType::END:
	{
		SC_EndPacket EndPacket;
		EndPacket.m_clientScore[0] = client[0].m_clientScore;
		EndPacket.m_clientScore[1] = client[1].m_clientScore;
		EndPacket.m_clientScore[2] = client[2].m_clientScore;
		retval = send(client_sock, (char*)&EndPacket, sizeof(SC_EndPacket), 0);
		break;
	}
	}
	return retval;
}

int CS_RecvData(SOCKET client_sock, int clientPID)
{
	int retval;
	int Trash;
	switch (client[clientPID].m_packetType)
	{
		case PacketType::NONE:
		{
			printf("Packet Type Error!\n");
			retval = recv(client_sock, (char*)&Trash, sizeof(Trash), MSG_WAITALL);
			break;
		}
		case PacketType::LOBBY:
		{
			retval = recv(client_sock, (char*)&(client[clientPID].m_clientReady), sizeof(bool), MSG_WAITALL);
			break;
		}
		case PacketType::MAIN:
		{
			retval = recv(client_sock, (char*)&client[clientPID].m_clientNextAngle, sizeof(float), MSG_WAITALL);
			client[clientPID].m_clientNextAngle = AngleOverflow(client[clientPID].m_clientNextAngle);

			break;
		}
		case PacketType::END:
		{
			retval = recv(client_sock, (char*)&(client[clientPID].m_ExitEndScene), sizeof(bool), MSG_WAITALL);
			break;
		}
	}
	return retval;
}

void ChangetoLobby()
{
	int tempCnt = 0;
	for (int i = 0; i < PLAYERNUM; ++i)
	{
		if (client[i].m_ExitEndScene == true)
		{
			client[i].m_packetType = LOBBY;
			tempCnt++;
		}
	}

	if (tempCnt == PLAYERNUM)
	{
		packetType = LOBBY;
		for (int i = 0; i < PLAYERNUM; ++i)
		{
			client[i].m_ExitEndScene = false;
		}
	}

}

void ChangePacket(PacketType pType)
{
	packetType = pType;
	for (int i = 0; i < PLAYERNUM; ++i)
	{
		if (pType == MAIN) client[i].m_clientReady = false;
		client[i].m_packetType = pType;
	}
}
