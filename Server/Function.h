#pragma once
#include <iostream>
#include "Global.h"

void InitBall();
void InitClient();
bool BarCollision();
bool EndCheck();

void UpdateBallData(double time);
void CalculateCollision();

// 추가부분
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

bool CheckPlayerReady();
bool CheckGameOver();

void CheckBarCollision();
void CheckBallCollision();
void Collision_Ball();
void CollisionDetect(struct Power_Orb* Orb);
bool DistanceOvercmp(float x, float y, float dis);
float AnglePosition(float x, float y);

int CS_RecvData(SOCKET client_sock, int clientPID);
int SC_SendVariableData(SOCKET client_sock, int clientPID);
bool Intersect(clientData, BallData);