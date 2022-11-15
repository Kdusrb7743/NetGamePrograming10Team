#pragma once
#include <iostream>
#include "Global.h"

void InitBall();
bool BarCollision();
bool EndCheck();

void UpdateBallData(double time);
void CalculateCollision();

void SetBallPosition();
void OrbSpeed();
void SpeedCaculate(double time);

bool CheckPlayerReady();
bool CheckGameOver();

void CheckBarCollision();
//void CheckBallCollision();
bool DistanceOvercmp(float x, float y, float dis);
float AnglePosition(float x, float y);

int SC_SendFixedData(SOCKET client_sock);
int CS_RecvData(SOCKET client_sock, int clientPID);
int SC_SendVariableData(SOCKET client_sock, int clientPID);