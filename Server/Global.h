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
#define PLAYERNUM 3

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
	PositionData m_BallPos;				// BallPosition					//----------�������� Ŭ�� �� ���� ��ġ���� ��
	float m_BallSpeed;					// 1Frame�� ���� ������ �Ÿ�
	float m_BallAngle;					// ���� ���� ����
	short m_BallCount = 3;					// ���� ����
	float m_BallSpeedx, m_BallSpeedy;	// ���� ������ ���� x, y ���� ��ġ �̵���

	float size = 30;					// ���� ���� ũ�� = ����30
};

struct clientData					// �������� �۾��� ��, �ʿ��� Ŭ���̾�Ʈ(User) ������
{
	PositionData m_clientPos;		// ������ ��ǥ
	PositionData m_clientNextPos;	// Ŭ���̾�Ʈ���� ���Ź��� ���� ��ǥ���� �ӽ÷� �����ϴ� ����
	float m_clientAngle;			// �г� �߽ɿ� �ش��ϴ� ������
	bool m_clientReady = false;
	unsigned short m_clientScore;
	char m_clientColor;				// RGB
};

static HANDLE clientFlag[PLAYERNUM];	// Static ������ Flag ������ �� ������ ����
static HANDLE processFlag;
static PacketType packetType = LOBBY;
static clientData client[PLAYERNUM];