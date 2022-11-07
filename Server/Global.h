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

struct BallData							// �������� �۾��� ��, �ʿ��� Ball ������
{
	PositionData m_BallPos;				// BallPosition					//----------�������� Ŭ�� �� ���� ��ġ���� ��
	float m_BallSpeed;					// 1Frame�� ���� ������ �Ÿ�
	float m_BallAngle;					// ���� ���� ����
	short m_BallCount;					// ���� ����
	float m_BallSpeedx, m_BallSpeedy;	// ���� ������ ���� x, y ���� ��ġ �̵���

	float size = 30;					// ���� ���� ũ�� = ����30
};

BallData Ball;

PacketType packetType(LOBBY);