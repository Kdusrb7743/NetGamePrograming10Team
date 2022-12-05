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

struct BallData							// �������� �۾��� ��, �ʿ��� Ball ������
{
	PositionData m_BallPos = {0.f, 0.f};				// BallPosition					//----------�������� Ŭ�� �� ���� ��ġ���� ��
	float m_BallSpeed = 1.f;					// 1Frame�� ���� ������ �Ÿ�
	float m_BallAngle;					// ���� ���� ����
	short m_BallCount = 3;					// ���� ����
	float m_BallSpeedx, m_BallSpeedy;	// ���� ������ ���� x, y ���� ��ġ �̵���

	float size = 30.f;					// ���� ���� ũ�� = ����30
};

struct clientData					// �������� �۾��� ��, �ʿ��� Ŭ���̾�Ʈ(User) ������
{
	PositionData m_clientPos;		// ������ ��ǥ
	PositionData m_clientNextPos;	// Ŭ���̾�Ʈ���� ���Ź��� ���� ��ǥ���� �ӽ÷� �����ϴ� ����
	float m_clientAngle;			// �г� �߽ɿ� �ش��ϴ� ������
	float m_clientNextAngle;		// �г� �߽ɿ� �ش��ϴ� ������
	bool m_clientReady = false;
	unsigned short m_clientScore;
	char m_clientColor;				// RGB
	bool m_ExitEndScene = false;
	PacketType m_packetType = LOBBY;
};

#pragma pack(1)
struct CS_LobbyPacket
{
	bool m_clientReady = false;
};
#pragma pack()

#pragma pack(1)
struct CS_MainPacket
{
	PositionData m_clientPos;
};
#pragma pack()

#pragma pack(1)
struct CS_ENDPacket
{
	bool m_ExitEndScene = false;
};
#pragma pack()

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
	short m_ballcount = 3;				// ��ī��Ʈ
	float m_clientPos[3];					// ���� �̿��� �÷��̾� ����
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