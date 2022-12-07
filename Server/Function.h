#pragma once
#include <iostream>
#include "Global.h"

void InitBall();
void InitClient();
void BarCollision();

void UpdateBallData(double time);
void CalculateCollision();

bool AngleDetect(float x, float y, float Angle);
float AngleOverflow(float Angle);
bool Distancecmp(float x, float y, float dis);
bool DistanceDetect(float x, float y, float Angle, float Distance, float Size);
void ReflectOrb(float Angle);
bool ObtuseDetect(float Angle);
float Reflect(float Angle, float Reflector);

void SetBallPosition();
void OrbSpeed();
void SpeedCaculate(double time);

bool ReadyCheck();
bool EndCheck();

bool DistanceOvercmp(float x, float y, float dis);
float AnglePosition(float x, float y);

int CS_RecvData(SOCKET client_sock, int clientPID);
int SC_SendData(SOCKET client_sock, int clientPID);

void ChangetoLobby();
void ChangePacket(PacketType pType);