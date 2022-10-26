#include "Power_Math.h"
const float M_TU = M_PI * 2 , Kelvin = 273.15, MaxTemp = 1000, MaxMole = 500, MaxPressure = 100000;
float PointRotationX(float x, float y, float angle)									//
{
	return x * cos(M_TU * angle) - y * sin(M_TU * angle);
}
float PointRotationY(float x, float y, float angle)
{
	return x * sin(M_TU * angle) + y * cos(M_TU * angle);
}
float PressureCaculate(float Mole, float Temperture)
{
	return Mole * Temperture / MaxPressure;
}
float SpeedCaculate(float Power, float Mole, float Temperture)
{
	return Power * (1 - (Mole / MaxMole));
}
float OrbScore(float Speed, float Mole, float Pressure, float Crits, bool Cherenkov)	// 스피드에 따라 점수를 얻는다.
{
	Speed = Speed * Speed * 100;
	if (Mole <= 0) return Speed * 0.75 * Crits;
	else if (Pressure < 0.5) return (Speed * (Pressure + 0.25) + (Speed * Pressure * Cherenkov)) * Crits;
	else return (Speed + (Speed * Cherenkov)) * Crits;
}
float AngleOverflow(float Angle)									// 각도 360초과시 다시 0으로 돌린다.
{
	if (Angle >= 1)
		Angle--;
	else if (Angle < 0)
		Angle++;
	return Angle;
}
float AnglePosition(float x, float y)								//
{
	return atan2( - y, - x) / M_TU + 0.5;
}
float Reflect(float Angle, float Reflector)
{
	Angle = 0.5 - Angle + Reflector * 2;
	return Angle;
}
float DistancePosition(float x, float y)
{
	return sqrt((x * x) + (y * y));
}
int PNcmp(float num)
{
	if (num > 0) return 1;
	else if (num < 0) return -1;
	else return 0;
}
bool Distancecmp(float x, float y, float dis)
{
	return ((x * x) + (y * y) < dis * dis);
}
bool DistanceOvercmp(float x, float y, float dis)
{
	return ((x * x) + (y * y) > dis * dis);
}
bool DistanceDetect(float x, float y, float Angle, float Distance, float Size)
{
	return (x * x + y * y > (Distance * Distance - Size * Size * 0.25) / (cos(Angle * M_TU) * cos(Angle * M_TU)));
}
bool AngleDetect(float x, float y, float Angle)
{	
	Angle = AngleOverflow(AnglePosition(x, y) - Angle);
	return (Angle < 0.13) || (0.87 < Angle);
}
bool ObtuseDetect(float Angle)
{
	AngleOverflow(Angle);
	return ((0.125 < Angle && Angle <= 0.25) || (0.75 <= Angle && Angle < 0.875));
}