#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h> 

#pragma comment(lib, "ws2_32")

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    16384

HANDLE clientFlag[3];
HANDLE processFlag;

enum PacketType
{
	LOBBY,
	MAIN,
	END
};

struct PositionData
{
	float x = 0.f, y = 0.f;
};

struct BallData							// 서버에서 작업할 때, 필요한 Ball 데이터
{
	PositionData m_BallPos;				// BallPosition					//----------서버에서 클라 줄 때는 위치값만 줌
	float m_BallSpeed;					// 1Frame당 공이 움직일 거리
	float m_BallAngle;					// 공의 진행 각도
	short m_BallCount;					// 공의 개수
	float m_BallSpeedx, m_BallSpeedy;	// 공의 각도에 따른 x, y 다음 위치 이동량

	float size = 30;					// 공의 실제 크기 = 고정30
};

BallData Ball;

PacketType packetType(LOBBY);