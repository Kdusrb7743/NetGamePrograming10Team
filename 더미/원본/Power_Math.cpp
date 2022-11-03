#include "Power_Math.h"

//-----------------------------------------------------------------
// ��� ���̳�, �гε��� ����� ��� (0, 0) ���� �Ÿ��� ������ ���
//------------------------------------------------------------------

//-------------------------------------------------------
//��������
const float M_TU = M_PI * 2 , Kelvin = 273.15, MaxTemp = 1000, MaxMole = 500, MaxPressure = 100000;
//-------------------------------------------------------

float PointRotationX(float x, float y, float angle)					// �̹��� ȸ�� ����� ���� X��ǥ ���
{
	return x * cos(M_TU * angle) - y * sin(M_TU * angle);
}
float PointRotationY(float x, float y, float angle)					// �̹��� ȸ�� ����� ���� y��ǥ ���
{
	return x * sin(M_TU * angle) + y * cos(M_TU * angle);
}
//float PressureCaculate(float Mole, float Temperture)								// �з¿� ���� ��� - ����
//{
//	return Mole * Temperture / MaxPressure;
//}
float SpeedCaculate(float Power)						// ���ڷξ翡 ���� ��� - ����
{
	return Power;
}
//float OrbScore(float Speed, float Mole, float Pressure, float Crits, bool Cherenkov)// ���ǵ�,�з�(����),���ڷ�(����)�� ���� ������ ��´�.
//{
//	Speed = Speed * Speed * 100;
//	//if (Mole <= 0) return Speed * 0.75 * Crits;
//	//else if (Pressure < 0.5) return (Speed * (Pressure + 0.25) + (Speed * Pressure * Cherenkov)) * Crits;
//	return (Speed + (Speed * Cherenkov)) * Crits;
//}
float OrbScore(float Speed)// ���ǵ�,�з�(����),���ڷ�(����)�� ���� ������ ��´�.
{
	Speed = Speed * Speed * 100;
	//if (Mole <= 0) return Speed * 0.75 * Crits;
	//else if (Pressure < 0.5) return (Speed * (Pressure + 0.25) + (Speed * Pressure * Cherenkov)) * Crits;
	return Speed;
}
float AngleOverflow(float Angle)									// ���� 360�ʰ��� �ٽ� 0���� ������.
{
	if (Angle >= 1)
		Angle--;
	else if (Angle < 0)
		Angle++;
	return Angle;
}

float AnglePosition(float x, float y)								// ���� ������ ��ȯ?????
{
	return atan2( - y, - x) / M_TU + 0.5;							// ���� ���� / 2���� + 0.5?
}

float Reflect(float Angle, float Reflector)							// �ݻ�� ������ ��ȯ(ƨ�� �� �� ������ ����)
{
	Angle = 0.5 - Angle + Reflector * 2;
	return Angle;
}

float DistancePosition(float x, float y)							// �������� ������ �Ÿ� ��ȯ - ���� (���̴� �� ����)
{
	return sqrt((x * x) + (y * y));
}

//int PNcmp(float num)												// ��ư Ŭ�� ���� �Լ� - ����
//{
//	if (num > 0) return 1;
//	else if (num < 0) return -1;
//	else return 0;
//}

bool Distancecmp(float x, float y, float dis)						// �Ÿ��� ���� �����̸� Ʈ��
{
	return ((x * x) + (y * y) < dis * dis);
}
bool DistanceOvercmp(float x, float y, float dis)					// �Ÿ��� ���� ������ ������ �� Ʈ��
{
	return ((x * x) + (y * y) > dis * dis);
}
bool DistanceDetect(float x, float y, float Angle, float Distance, float Size)	// �浹�� - �Ÿ����� ����
{
	return (x * x + y * y > (Distance * Distance - Size * Size * 0.25) / (cos(Angle * M_TU) * cos(Angle * M_TU)));
}
bool AngleDetect(float x, float y, float Angle)						// �浹�� - �гΰ��� ���� ����
{	
	Angle = AngleOverflow(AnglePosition(x, y) - Angle);
	return (Angle < 0.13) || (0.87 < Angle);
}
bool ObtuseDetect(float Angle)										// �а����� �浹��
{
	AngleOverflow(Angle);
	return ((0.125 < Angle && Angle <= 0.25) || (0.75 <= Angle && Angle < 0.875));
}