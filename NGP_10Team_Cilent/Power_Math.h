#ifndef _POWER_MATH_
#define _POWER_MATH_
#define _USE_MATH_DEFINES
#include <math.h>
extern const float M_TU, Kelvin, MaxTemp, MaxMole, MaxPressure;
float PointRotationX(float x, float y, float angle);
float PointRotationY(float x, float y, float angle);
float PressureCaculate(float Mole, float Temperture);
float SpeedCaculate(float Power, float Mole, float Temperture);
float OrbScore(float Speed, float Mole, float Pressure, float Crits, bool Cherenkov);
float AngleOverflow(float Angle);
float AnglePosition(float x, float y);
float Reflect(float Angle, float Reflector);
float DistancePosition(float x, float y);
int PNcmp(float num);
bool Distancecmp(float x, float y, float dis);
bool DistanceOvercmp(float x, float y, float dis);
bool DistanceDetect(float x, float y, float Angle, float Distance, float Size);
bool AngleDetect(float x, float y, float Angle);
bool ObtuseDetect(float Angle);
#endif