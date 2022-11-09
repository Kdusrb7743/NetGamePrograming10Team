#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h> 

#pragma comment(lib, "ws2_32")

#define SERVERPORT 9000
#define BUFSIZE    16384

enum Packet_Type
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
