#pragma once
#include <iostream>
#include "Global.h"

void InitBall();
bool BarCollision();
bool EndCheck();

void UpdateBallData();
void CalculateCollision();

struct BallData* OrbPosition(struct BallData* Orb);

bool CheckPlayerReady();
bool CheckGameOver();

int SC_SendFixedData(SOCKET client_sock);
int CS_RecvData(SOCKET client_sock, int clientPID);