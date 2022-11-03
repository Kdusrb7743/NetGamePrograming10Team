#include "Power_Math.h"

//-----------------------------------------------------------------
// 모든 공이나, 패널들은 경기장 가운데 (0, 0) 기준 거리와 각도를 계산
//------------------------------------------------------------------

//-------------------------------------------------------
//전역변수
const float M_TU = M_PI * 2 , Kelvin = 273.15, MaxTemp = 1000, MaxMole = 500, MaxPressure = 100000;
//-------------------------------------------------------

float PointRotationX(float x, float y, float angle)					// 이미지 회전 출력을 위한 X좌표 계산
{
	return x * cos(M_TU * angle) - y * sin(M_TU * angle);
}
float PointRotationY(float x, float y, float angle)					// 이미지 회전 출력을 위한 y좌표 계산
{
	return x * sin(M_TU * angle) + y * cos(M_TU * angle);
}
//float PressureCaculate(float Mole, float Temperture)								// 압력에 따른 계산 - 삭제
//{
//	return Mole * Temperture / MaxPressure;
//}
float SpeedCaculate(float Power)						// 원자로양에 따른 계산 - 삭제
{
	return Power;
}
//float OrbScore(float Speed, float Mole, float Pressure, float Crits, bool Cherenkov)// 스피드,압력(삭제),원자량(삭제)에 따라 점수를 얻는다.
//{
//	Speed = Speed * Speed * 100;
//	//if (Mole <= 0) return Speed * 0.75 * Crits;
//	//else if (Pressure < 0.5) return (Speed * (Pressure + 0.25) + (Speed * Pressure * Cherenkov)) * Crits;
//	return (Speed + (Speed * Cherenkov)) * Crits;
//}
float OrbScore(float Speed)// 스피드,압력(삭제),원자량(삭제)에 따라 점수를 얻는다.
{
	Speed = Speed * Speed * 100;
	//if (Mole <= 0) return Speed * 0.75 * Crits;
	//else if (Pressure < 0.5) return (Speed * (Pressure + 0.25) + (Speed * Pressure * Cherenkov)) * Crits;
	return Speed;
}
float AngleOverflow(float Angle)									// 각도 360초과시 다시 0으로 돌린다.
{
	if (Angle >= 1)
		Angle--;
	else if (Angle < 0)
		Angle++;
	return Angle;
}

float AnglePosition(float x, float y)								// 절대 각도를 반환?????
{
	return atan2( - y, - x) / M_TU + 0.5;							// 절대 각도 / 2파이 + 0.5?
}

float Reflect(float Angle, float Reflector)							// 반사된 각도를 반환(튕길 때 공 각도에 적용)
{
	Angle = 0.5 - Angle + Reflector * 2;
	return Angle;
}

float DistancePosition(float x, float y)							// 원점으로 부터의 거리 반환 - 삭제 (쓰이는 곳 삭제)
{
	return sqrt((x * x) + (y * y));
}

//int PNcmp(float num)												// 버튼 클릭 계산용 함수 - 삭제
//{
//	if (num > 0) return 1;
//	else if (num < 0) return -1;
//	else return 0;
//}

bool Distancecmp(float x, float y, float dis)						// 거리가 레일 안쪽이면 트루
{
	return ((x * x) + (y * y) < dis * dis);
}
bool DistanceOvercmp(float x, float y, float dis)					// 거리가 레일 밖으로 나갔을 때 트루
{
	return ((x * x) + (y * y) > dis * dis);
}
bool DistanceDetect(float x, float y, float Angle, float Distance, float Size)	// 충돌시 - 거리감지 계산용
{
	return (x * x + y * y > (Distance * Distance - Size * Size * 0.25) / (cos(Angle * M_TU) * cos(Angle * M_TU)));
}
bool AngleDetect(float x, float y, float Angle)						// 충돌시 - 패널각도 감지 계산용
{	
	Angle = AngleOverflow(AnglePosition(x, y) - Angle);
	return (Angle < 0.13) || (0.87 < Angle);
}
bool ObtuseDetect(float Angle)										// 둔각으로 충돌시
{
	AngleOverflow(Angle);
	return ((0.125 < Angle && Angle <= 0.25) || (0.75 <= Angle && Angle < 0.875));
}