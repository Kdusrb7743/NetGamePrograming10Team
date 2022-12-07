#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h> 

#include "DirectXCollision.h"

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

struct BallData			
{
	PositionData m_BallPos = {0.f, 0.f};
	float m_BallSpeed = 1.f;			
	float m_BallAngle;				
	short m_BallCount = 3;			
	float m_BallSpeedx, m_BallSpeedy;
	float size = 30.f;
};

struct clientData
{
	float m_clientAngle;			
	float m_clientNextAngle;	
	bool m_clientReady = false;
	unsigned short m_clientScore;
	bool m_ExitEndScene = false;
	PacketType m_packetType = LOBBY;
	int m_ReflectDelayTime = 0;
};

#pragma pack(1)
struct SC_LobbyPacket
{
	PacketType ptype = LOBBY;
	char m_clientPID;
};
#pragma pack()

#pragma pack(1)
struct SC_MainPacket
{
	PacketType ptype = MAIN;
	PositionData m_ballPos;
	short m_ballcount = 3;
	float m_clientPos[3];
	unsigned short m_clientScore;
};
#pragma pack()

#pragma pack(1)
struct SC_EndPacket
{
	PacketType ptype = END;
	unsigned short m_clientScore[3];
};
#pragma pack()