#include "Function.h"
#include <random>

random_device rnd;
uniform_real_distribution<float> dist(0.f, 1.f);  // ���� [0.0, 1.0)


BallData Ball;

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
	return false;
}

bool EndCheck() {			// ������ ���������� üũ
	if (Ball.m_BallCount == 0) return true;
	else return false;
}

void UpdateBallData()
{

}

void CalculateCollision()
{

}

struct BallData* OrbPosition(struct BallData* Orb)
{

	Orb->m_BallPos.x += Orb->m_BallSpeedx;
	Orb->m_BallPos.y += Orb->m_BallSpeedy;

	return Orb;
}

bool CheckPlayerReady(const clientData* client)
{
	int readyPlayerCNT = 0;
	for (int i = 0; i < PLAYERNUM; i++)
	{
		if (client++->m_clientReady)
		{
			readyPlayerCNT++;
			// printf("Player %d is ready\n", i + 1);
		}
		//else
			// printf("Player %d is not ready\n", i + 1);
	}
	if (readyPlayerCNT == PLAYERNUM)
		return true;

	return false;
}

bool CheckGameOver()
{
	if (Ball.m_BallCount == 0)
	{
		return true;
	}
	return false;
}
