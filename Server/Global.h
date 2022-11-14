#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h> 

#pragma comment(lib, "ws2_32")

using namespace std;

#define SERVERPORT	9000
#define BUFSIZE		16384
#define PLAYERNUM	3
#define PI			3.14159265358979323846

enum PacketType
{
	NONE,
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
	PositionData m_BallPos = {0.f, 0.f};				// BallPosition					//----------서버에서 클라 줄 때는 위치값만 줌
	float m_BallSpeed = 0.f;					// 1Frame당 공이 움직일 거리
	float m_BallAngle;					// 공의 진행 각도
	short m_BallCount = 3;					// 공의 개수
	float m_BallSpeedx, m_BallSpeedy;	// 공의 각도에 따른 x, y 다음 위치 이동량

	float size = 30;					// 공의 실제 크기 = 고정30
};

struct clientData					// 서버에서 작업할 때, 필요한 클라이언트(User) 데이터
{
	PositionData m_clientPos;		// 중점의 좌표
	PositionData m_clientNextPos;	// 클라이언트에게 수신받은 다음 좌표값을 임시로 저장하는 변수
	float m_clientAngle;			// 패널 중심에 해당하는 각도값
	bool m_clientReady = false;
	unsigned short m_clientScore;
	char m_clientColor;				// RGB
};

struct CS_LobbyPacket
{
	bool m_clientReady = false;
};

struct CS_MainPacket
{
	PositionData m_clientPos;
};

struct SC_LobbyPacket
{
	char m_clientColor[3];
};

struct SC_MainPacket
{
	PositionData m_ballPos;
	PositionData m_clientPos[3];	// 본인 이외의 플레이어 좌표
	unsigned short m_clientScore;
};

struct SC_EndPacket
{
	unsigned short m_clientScore[3];
};

