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
	//Ball.m_BallPos.x = cos((2 * PI) * Ball.m_BallAngle);
	//Ball.m_BallPos.y = sin((2 * PI) * Ball.m_BallAngle);
	Ball.m_BallPos.x = 0;
	Ball.m_BallPos.y = 0;
	Ball.m_BallSpeed = 1.f;				// 기본 스피드
	//Ball.m_BallSpeedx = cos((2 * PI) * Ball.m_BallAngle);				// 매 공이 움직이기 전 각도에 따른 x이동량
	//Ball.m_BallSpeedy = sin((2 * PI) * Ball.m_BallAngle);
	Ball.m_BallSpeedx = Ball.m_BallSpeed * cos((2 * PI) * Ball.m_BallAngle) * 5;
	Ball.m_BallSpeedy = Ball.m_BallSpeed * sin((2 * PI) * Ball.m_BallAngle) * 5;
}

void InitClient()
{
	client[0].m_clientAngle = 0.25f;
	client[1].m_clientAngle = 0.583f;
	client[2].m_clientAngle = 0.915f;
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
	//SpeedCaculate(time);
	//OrbSpeed();
	SetBallPosition();
	//printf("BallPos (X: %f, Y: %f)\n", Ball.m_BallPos.x, Ball.m_BallPos.y);
}

void CalculateCollision()
{
	//CheckBarCollision();
	//CheckBallCollision();
	// 

	if (DistanceOvercmp(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, 375 - 110) &&
		AngleDetect(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, client[0].m_clientAngle) &&
		Distancecmp(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, 375) &&
		DistanceDetect(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, AnglePosition(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0) - client[0].m_clientAngle, 375, 375))
	{
		//ReflectReflector(Orb, Reflector);
		ReflectOrb(client[0].m_clientAngle);						// 공의 각도 변경
		//OrbPosition(Orb);										// 공의 위치 변경
		Ball.m_BallPos.x += Ball.m_BallSpeedx;
		Ball.m_BallPos.y += Ball.m_BallSpeedy;
		cout << "튕겼다는 증거" << endl;
	}

	else if (DistanceOvercmp(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, 375 - 110) &&
		AngleDetect(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, client[1].m_clientAngle) &&
		Distancecmp(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, 375) &&
		DistanceDetect(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, AnglePosition(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0) - client[1].m_clientAngle, 375, 375))
	{
		//ReflectReflector(Orb, Reflector);
		ReflectOrb(client[1].m_clientAngle);						// 공의 각도 변경
		//OrbPosition(Orb);										// 공의 위치 변경
		Ball.m_BallPos.x += Ball.m_BallSpeedx;
		Ball.m_BallPos.y += Ball.m_BallSpeedy;
		cout << "튕겼다는 증거" << endl;
	}

	else if (DistanceOvercmp(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, 375 - 110) &&
		AngleDetect(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, client[2].m_clientAngle) &&
		Distancecmp(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, 375) &&
		DistanceDetect(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0, AnglePosition(Ball.m_BallPos.x + 0, Ball.m_BallPos.y + 0) - client[2].m_clientAngle, 375, 375))
	{
		//ReflectReflector(Orb, Reflector);
		ReflectOrb(client[2].m_clientAngle);						// 공의 각도 변경
		//OrbPosition(Orb);										// 공의 위치 변경
		Ball.m_BallPos.x += Ball.m_BallSpeedx;
		Ball.m_BallPos.y += Ball.m_BallSpeedy;
		cout << "튕겼다는 증거" << endl;
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
	Ball.m_BallSpeed = 1.f;
	Ball.m_BallSpeedx = Ball.m_BallSpeed * cos((2 * PI) * Ball.m_BallAngle) * 5;
	Ball.m_BallSpeedy = Ball.m_BallSpeed * sin((2 * PI) * Ball.m_BallAngle) * 5;

	//Ball.m_BallSpeedx = Ball.m_BallSpeed * cos(2 * PI * (1.f - Ball.m_BallAngle)) * 5;
	//Ball.m_BallSpeedy = Ball.m_BallSpeed * sin(2 * PI * (1.f - Ball.m_BallAngle)) * 5;

	//Ball.m_BallAngle = 1.f - ((atan2(-Ball.m_BallPos.y, -Ball.m_BallPos.x) / (2 * PI)) + 0.5);
	//printf("BallAngle : %f\n", Ball.m_BallAngle);
}

void SpeedCaculate(double time)
{
	if (time < FLT_MIN)
	{
		Ball.m_BallSpeed = 0.0001f;
	}
	else
	{
		Ball.m_BallSpeed = time * 0.001f;
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
		MainPacket.m_clientScore = client[clientPID].m_clientScore;							//자기 스코어

		retval = send(client_sock, (char*)&MainPacket, sizeof(SC_MainPacket), 0);		// 각도값 보내기
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
			//retval = 10;				// 이부분 나중에 작업할 때 없앤다.
			// retval = recv(client_sock, (char*)&client[clientPID].m_clientNextPos, sizeof(CS_MainPacket), MSG_WAITALL);
			retval = recv(client_sock, (char*)&client[clientPID].m_clientAngle, sizeof(float), MSG_WAITALL);
			//cout << clientPID << "번 클라이언트 각도 : " << client[0].m_clientAngle << endl;
			//cout << clientPID << "번 클라이언트 각도 : " << client[1].m_clientAngle << endl;
			//cout << clientPID << "번 클라이언트 각도 : " << client[2].m_clientAngle << endl;
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
