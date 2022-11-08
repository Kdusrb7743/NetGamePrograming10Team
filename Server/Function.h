#pragma once
#include "Global.h"

void InitBall();
bool BarCollision();
bool EndCheck();

void UpdateBallData();
void CalculateCollision();

struct BallData* OrbPosition(struct BallData* Orb);

bool CheckPlayerReady(const clientData* client);
bool CheckGameOver();