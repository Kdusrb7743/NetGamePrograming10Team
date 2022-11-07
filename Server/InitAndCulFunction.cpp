#pragma once
#include "Global.h"
#include <random>

random_device rnd;
uniform_real_distribution<float> dist(0.f, 1.f);  // ���� [0.0, 1.0)

void InitBall()		// �� �ӵ� �� ����(����) �ʱ�ȭ, ���� �� ���� ����
{
	Ball.m_BallPos.x = 0;
	Ball.m_BallPos.y = 0;
	Ball.m_BallAngle = dist(rnd);			// �Ź� ������
	Ball.m_BallSpeed = 1;				// �⺻ ���ǵ�
	Ball.m_BallCount = 3;				// �⺻ �� ����
	Ball.m_BallSpeedx = 0;				// �� ���� �����̱� �� ������ ���� x�̵���
	Ball.m_BallSpeedy = 0;

}

bool BarCollision() {		// �ݻ���(�÷��̾�)�� �浹�� ����

}

bool EndCheck() {			// ������ ���������� üũ
	if (Ball.m_BallCount == 0) return true;
	else return false;
}
