#include "Power_ing.h"
//----------------------------------------------
// 현 cpp에 Orb(공)과 패널(반사판)을 계산
//----------------------------------------------

//--------------------------------------------------------------------------------------------------------------//
bool GameStart;
float Score, Temperture, Mole, TotalScore;
int Time, PreTime, ReactorEffect, Button[5], OrbType, Orbcount;
struct Power_Cherenkov Cherenkov;
struct Power_Orb* OrbHead = (Power_Orb*)malloc(sizeof(struct Power_Orb));
struct Power_Reflector* ReflectorHead = (Power_Reflector*)malloc(sizeof(struct Power_Reflector));
//--------------------------------------------------------------------------------------------------------------//
void CherenkovCheck()													// 게임 피버모드 - 삭제
{
	if (Cherenkov.cherenkov)
	{
		if (ReactorEffect < 5 && Time % 3 == 0)	ReactorEffect ++;
		if (Cherenkov.counter > 0) Cherenkov.counter = 0;
		if (Cherenkov.meter > 0)
			Cherenkov.meter -= 2;
		else
		{
			Cherenkov.lever = 5;
			Cherenkov.cherenkov = false;
		}
	}
	else
	{
		if (Cherenkov.levertrigger == false && Cherenkov.lever > 0 && Time % 3 == 0) Cherenkov.lever--;
		if (Cherenkov.lever == 6) (Cherenkov.cherenkov = true);
		if (ReactorEffect > 0 && ReactorEffect < 6 && Time % 3 == 0)
		{
			ReactorEffect--;
		}
		if (Cherenkov.meter > 1000)
		{
			Cherenkov.counter = 0;
			Cherenkov.meter = 1000;
		}
		else if (Cherenkov.counter > 0)
		{
			Cherenkov.meter++;
			Cherenkov.counter--;
		}
	}
}
void GeneralReset()															// 공, 버튼, 시간, 씬, 점수, 이펙트 타임 초기화
{
	OrbClear(OrbHead);
	Cherenkov.cherenkov = false, Cherenkov.meter = 0, Cherenkov.counter = 0, Cherenkov.lever = 0;
	for (int i = 0; i < 5; i++) Button[i] = 0;
	Time = 0, GameStart = false, Score = 0, ReactorEffect = 0, Temperture = Kelvin, Mole = MaxMole * 0.5;
}
void GameRestart()														// 점수, 시간 초기화 게임 재시작
{
	if (RestartPressure)
	{
		Temperture -= (Temperture - Kelvin) / (31.0 + Time);
		Mole -= (Mole - MaxMole / 2) / (31.0 + Time);
	}
	Cherenkov.meter -= Cherenkov.meter / (31.0 + Time);
	Score -= Score / (31.0 + Time);
	if (Time % 5 == 0) ReactorEffect--;
	Time--;
}
void ButtonActive()															// 버튼 작동 (이미지) 및 출력
{
	if ((Button[1] < 0 && Temperture + Button[1] > Kelvin) || (Button[1] > 0 && Temperture + Button[1] < MaxTemp && PressureCaculate(Mole, Temperture + Button[1]) < 1)) Temperture += Button[1];
	else Button[1] = 0;
	if ((Button[2] < 0 && Mole + Button[2] >= 0) || (Button[2] > 0 && Mole + Button[2] < MaxMole && PressureCaculate(Mole + Button[2], Temperture) < 1)) Mole += Button[2];
	else Button[2] = 0;
	if (Button[3] > 0) Button[3]--;
	if (Button[4] > 0) Button[4]--;
}
//--------------------------------------------------------------------------------------------------------------//
bool PressureCheck()													// 온도체크 - 삭제
{
	return (PressureCaculate(Mole, Temperture) <= 0.875 && (PressureCaculate(Mole, Temperture) >= 0.375 || PressureCaculate(Mole, Temperture) <= 0));
}
//--------------------------------------------------------------------------------------------------------------//
struct Power_Orb* OrbPosition(struct Power_Orb* Orb)					// 스피드에 따른 공 위치 계산
{
	if (Cherenkov.cherenkov)
	{
		Orb->x += Orb->speedx * 0.5;
		Orb->y += Orb->speedy * 0.5;
	}
	else
	{
		Orb->x += Orb->speedx;
		Orb->y += Orb->speedy;
	}
	//--------------------------------오브 잔상 매번 배열에 위치값 넣기

	if (Orb->major)												//잔상 이미지 계산
	{
		for (int i = 0; i < Orb->effect_count; i++) {
			Orb->afterx[Orb->effect_count - i] = Orb->afterx[Orb->effect_count - i - 1];
			Orb->aftery[Orb->effect_count - i] = Orb->aftery[Orb->effect_count - i - 1];
		}
		Orb->afterx[0] = Orb->x;
		Orb->aftery[0] = Orb->y;
		if (Orb->effect_count < 24) Orb->effect_count++;
	}
	else
	{
		for (int i = 0; i < Orb->effect_count; i++) {
			Orb->afterx[Orb->effect_count - i] = Orb->afterx[Orb->effect_count - i - 1];
			Orb->aftery[Orb->effect_count - i] = Orb->aftery[Orb->effect_count - i - 1];
		}
		Orb->afterx[0] = Orb->x;
		Orb->aftery[0] = Orb->y;
		if (Orb->effect_count < 24) Orb->effect_count++;
	}
	
	//---------------------------------
	return Orb;
}
struct Power_Orb* OrbSpeed(struct Power_Orb* Orb)						// 공 스피드
{
	Orb->speed = SpeedCaculate(Orb->power, Mole, Temperture);
	Orb->speedx = Orb->speed * cos(M_TU * Orb->angle) * 5;
	Orb->speedy = Orb->speed * sin(M_TU * Orb->angle) * 5;
	Orb->shellx = cos(M_TU * Orb->angle) * Orb->size;
	Orb->shelly = sin(M_TU * Orb->angle) * Orb->size;
	return Orb;
}

void CollisionDetect(struct Power_Orb* Orb)								// 메인 공 충돌 했는지
{
	if (Orb->next != OrbHead)
	{
		OrbPosition(Orb->next);
		if (DistanceOvercmp(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, 500))
		{
			if (((Orb->next->major == false && Orb->next->type == 0) || Orb->next->effect == 1) && Distancecmp(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, 525))
			{
				ReflectOrb(Orb->next, AnglePosition(Orb->next->x, Orb->next->y));
				if (Orb->next->effect == 1) Orb->next->effect = 0;
			}
			else
			{
				if (Orb->next->major)
				{
					ReactorEffect = 6;
					if (Orbcount > 0) ReactorEffect = 6;
					else Orbcount = -1;
				}
				OrbRemove(Orb->next, Orb);
			}
		}
		CollisionDetect(Orb->next);
	}
	else return;
}
bool OrbMajorCheck(struct Power_Orb* Orb)						// 이 함수 쓰는 부분 없음(미완성), 메인 공 체크 - 삭제
{
	if (Orb->next != OrbHead)
	{
		if (Orb->next->major) return false;
		else OrbMajorCheck(Orb->next);
	}
	else return true;
}
void OrbCreate(struct Power_Orb* Orb, int Type, bool Major, float x, float y, float Angle)	//메인 씬에서 불러줌. 공 생성시 객체 초기화
{
	if (Orb->next == OrbHead)
	{
		Orb->next = new Power_Orb;
		Orb->next = OrbApply(Orb->next, Type, Major, x, y, Angle);
		Orb->next->next = OrbHead;
		return;
	}
	else OrbCreate(Orb->next, Type, Major, x, y, Angle);
}
void OrbRemove(struct Power_Orb* NextOrb, struct Power_Orb* Orb)							// 공 삭제
{
	Orb->next = Orb->next->next;
	delete NextOrb;
	return;
}
void OrbClear(struct Power_Orb* Orb)														// 공 전체 삭제
{
	if (Orb->next != OrbHead)
	{
		OrbRemove(Orb->next, Orb);
		OrbClear(Orb->next);
	}
	else return;
}

struct Power_Orb* OrbApply(struct Power_Orb* Orb, int Type, bool Major, float x, float y, float Angle) // 공 생성시 크기 초기화
{
	if (Major)													// 현재 공이 메인 공이면
	{
		switch (Type)
		{
		case 1:
			Orb->power = 1.5, Orb->size = 25, Orb->effect = 0;
			break;
		case 2:
			Orb->power = 2, Orb->size = 30, Orb->effect = 0;
			break;
		default:
			Orb->power = 2, Orb->size = 25, Orb->effect = 0;
			break;
		}
	}
	else														// 현재 공이 보조 공(점수용)
	{
		switch (Type)
		{
		case 1:
			Orb->power = 2, Orb->size = 18.75, Orb->effect = 0;
			break;
		case 2:
			Orb->power = 2, Orb->size = 18.75, Orb->effect = 0;
			break;
		default:
			Orb->power = 2, Orb->size = 18.75, Orb->effect = 0;
			break;
		}
	}
	Orb->x = x, Orb->y = y, Orb->type = Type, Orb->major = Major, Orb->angle = Angle, Orb->effect_count = 0;
	Orb = OrbSpeed(Orb);
	return Orb;
}
//--------------------------------------------------------------------------------------------------------------//
void ReflectDetect(struct Power_Orb* Orb, struct Power_Reflector* Reflector)		// 반사판의 충돌 감지
{
	if (Orb->next != OrbHead)					// 현재 플레이 씬, 공이 있는지
	{
		if (DistanceOvercmp(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, Reflector->position - 110) &&
			AngleDetect(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, Reflector->angle) && 
			Distancecmp(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, Reflector->position) &&
			DistanceDetect(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly, AnglePosition(Orb->next->x + Orb->next->shellx, Orb->next->y + Orb->next->shelly) - Reflector->angle, Reflector->position, Reflector->size))
			ReflectReflector(Orb, Reflector);
		ReflectDetect(Orb->next, Reflector);
	}
	else return;
}
void ReflectReflector(struct Power_Orb* Orb, struct Power_Reflector* Reflector)	// 반사판 충돌 검사 및 충돌하면 판에 대한 검수(점수, 튕김 애니메이션) 함수
{
	if (Orb->next->major)
	{
		if (Reflector->module_charged[0])
		{
			if (rand() % 3 == 0)
			{
				if (Reflector->module_charged[4])
				{
					OrbCreate(OrbHead, Reflector->module[4], false, Orb->next->x, Orb->next->y, Reflector->angle - 0.5);
					Reflector->module_charged[4] = false;
				}
				else OrbCreate(OrbHead, 0, false, Orb->next->x, Orb->next->y, Reflector->angle - 0.5);
			}
			ReflectReflectorOrb(Orb->next, Reflector);
			int score, Energy = 1, Cherenks = 1;
			if (Reflector->module_charged[2])
			{
				if (Reflector->module_charged[2] == 2) Cherenks = 1.5;
				else Energy = 1.25;
			}
			switch (Orb->next->type)
			{
			case 1:
				if (Reflector->age > -100) score = OrbScore(Orb->next->speed, Mole, PressureCaculate(Mole, Temperture), 1 * Energy, Cherenkov.cherenkov);
				else score = OrbScore(Orb->next->speed, Mole, PressureCaculate(Mole, Temperture), 0.5, Cherenkov.cherenkov);
				if (Cherenkov.meter < 1000 && Cherenkov.cherenkov == false)	Cherenkov.counter += 125 * Cherenks;
				break;
			case 2:
				if (Reflector->age > -100) score = OrbScore(Orb->next->speed, Mole, PressureCaculate(Mole, Temperture), 1.0125 * Energy, Cherenkov.cherenkov);
				else score = OrbScore(Orb->next->speed, Mole, PressureCaculate(Mole, Temperture), 0.75 * Energy, Cherenkov.cherenkov);
				if (Cherenkov.meter < 1000 && Cherenkov.cherenkov == false)	Cherenkov.counter += 100 * Orb->next->speed * Orb->next->speed * Cherenks;
				break;
			default:
				if (Reflector->age > -100) score = OrbScore(Orb->next->speed, Mole, PressureCaculate(Mole, Temperture), 1.35 * Energy, Cherenkov.cherenkov);
				else score = OrbScore(Orb->next->speed, Mole, PressureCaculate(Mole, Temperture), 1 * Energy, Cherenkov.cherenkov);
				if (Cherenkov.meter < 1000 && Cherenkov.cherenkov == false)	Cherenkov.counter += 125 * Cherenks;
				break;
			}
			CreateEffect(EffectHead, Orb->next->x, Orb->next->y, score);
			Score += score;
			if (score > 1000) Reflector->effect = 312;
			else if (score > 500) Reflector->effect = 212;
			else if (score > 250) Reflector->effect = 112;
			else Reflector->effect = 12;
			Reflector->age = (int)(50 / Orb->next->speed);
			Reflector->position += 15.0 * (int)Orb->next->speed;
		}
	}
	else
	{
		switch (Orb->next->type)
		{
		case 0:
			if (Reflector->module[0] != 0 && Reflector->module_charged[0] == false) Reflector->module_charged[0] = true;
			else if (ReflectorHead->next->module[1] != 0 && ReflectorHead->next->module_charged[1] == false) ReflectorHead->next->module_charged[1] = true;
			else if (Reflector->module[2] != 0 && Reflector->module_charged[2] == false) Reflector->module_charged[2] = true;
			else if (Reflector->module[3] != 0 && Reflector->module_charged[3] == false) Reflector->module_charged[3] = true;
			else if (Reflector->module[4] != 0 && Reflector->module_charged[4] == false) Reflector->module_charged[4] = true;
			else Score += OrbScore(Orb->next->speed, Mole, PressureCaculate(Mole, Temperture), 1, Cherenkov.cherenkov);
			break;
		case 1:
			if (Temperture < MaxTemp - 1)	Temperture++;
			else Temperture = MaxTemp;
			break;
		case 2:
			if (Temperture > 1)	Temperture--;
			else Temperture = 0;
			break;
		}
		OrbRemove(Orb->next, Orb);
	}
}
struct Power_Orb* ReflectReflectorOrb(struct Power_Orb* Orb, struct Power_Reflector* Reflector)	// 공과 패널의 충돌 검사 후 공위치 조정
{
	if (Reflector->module_charged[3])
	{
		if (Reflector->module[3] == 1) Orb->power += 0.1;
		else if (Reflector->module[3] == 2) Orb->power -= 0.1;
	}
	ReflectOrb(Orb, Reflector->angle);						// 공의 각도 변경
	OrbPosition(Orb);										// 공의 위치 변경
	return Orb;
}
struct Power_Orb* ReflectOrb(struct Power_Orb* Orb, float Angle)				// 반사된 공의 각도 변경
{
	if (ObtuseDetect(AngleOverflow(Orb->angle - Angle)))
	{
		if (AngleOverflow(Orb->angle - Angle) < 0.5) Orb->angle = AngleOverflow(Orb->angle + 0.25);
		else Orb->angle = AngleOverflow(Orb->angle - 0.25);
	}
	else Orb->angle = Reflect(Orb->angle, Angle);
	OrbSpeed(Orb);
	return Orb;
}
//--------------------------------------------------------------------------------------------------------------//
void ReflectorPosition(struct Power_Reflector* Reflector, short Left, short Right, short Up, short Down)	// 반사판의 위치를 매번 계산
{
	if (Reflector->next != ReflectorHead)
	{
		if (GameStart && Reflector->next->age <= 0) ReflectDetect(OrbHead, Reflector->next);
		if (Reflector->next->effect % 100 == 0) Reflector->next->effect = 0;
		else if ((Time & 1) && Reflector->next->effect > 0) Reflector->next->effect--;
		if (Reflector->next->age > -101) Reflector->next->age--;
		float Break = 1;
		if ((Up & 0x8001) || (Down & 0x8001) || (Up & 0x8000) || (Down & 0x8000) && Reflector->next->position < 455 && Reflector->next->position > 270)
		{
			if (ReflectorHead->next->module_charged[1])
			{
				if (Reflector->next->module_charged[1] == false) Reflector->next->module_charged[1] = true;
				switch (ReflectorHead->next->module[1])
				{
				case 1:
					if ((Up & 0x8000) && Reflector->next->speed < 2) Reflector->next->speed += 0.5;
					if ((Down & 0x8000) && Reflector->next->speed > 0) Reflector->next->speed -= 0.5;
					break;
				case 2:
					if ((Up & 0x8001) && Reflector->next->position > 300)  Reflector->next->position -= 15;
					if ((Down & 0x8001) && Reflector->next->position < 450) Reflector->next->position += 15;
					break;
				}
			}
			else
			{
				if (Up & 0x8001) Break = 2;
				if (Down & 0x8001) Break = 0.5;
			}
		}
		else
		{
			if (Reflector->next->position < 375) Reflector->next->position += 5;
			else if (Reflector->next->position > 500) Reflector->next->position -= (int)((Reflector->next->position * 0.2 - 75) * 0.2) * 5.0;
			else if (Reflector->next->position > 375) Reflector->next->position -= 5;
		}
		if (Right & 0x8001) Reflector->next->angle = AngleOverflow(Reflector->next->angle - 0.006 * Reflector->next->speed / Reflector->next->position * 375 * Break);
		if (Left & 0x8001) Reflector->next->angle = AngleOverflow(Reflector->next->angle + 0.006 * Reflector->next->speed / Reflector->next->position * 375 * Break);
		ReflectorPosition(Reflector->next, Left, Right, Up, Down);
	}
	else return;
}
void ReflectorCreate(struct Power_Reflector* Reflector, int Count)				// 반사판 생성시
{
	if (Reflector->next == ReflectorHead)
	{
		Reflector->next = new Power_Reflector;
		Reflector->next = ReflectorApply(Reflector->next, Count);
		Reflector->next->next = ReflectorHead;
		if (Count < Reflector->module[0])  ReflectorCreate(Reflector->next, Count + 1);
		else return;
	}
	else ReflectorCreate(Reflector->next, Count);
}
void ReflectorRemove(struct Power_Reflector* NextReflector, struct Power_Reflector* Reflector) //반사판 삭제
{
	Reflector->next = Reflector->next->next;
	delete NextReflector;
	return;
}
void ReflectClear(struct Power_Reflector* Reflector)											// 반사판 전체 삭제
{
	if (Reflector->next != ReflectorHead)
	{
		ReflectorRemove(Reflector->next, Reflector);
		ReflectClear(Reflector->next);
	}
	else return;
}
struct Power_Reflector* ReflectorReset(struct Power_Reflector* Reflector)						// 반사판 리셋 -- 게임 시작시, ESC일때 이미지를 삭제해줘야 해서 그때마다 리셋 후 다시 생성
{
	ReflectClear(ReflectorHead);
	Reflector->module[0] = 0, Reflector->module[1] = 0, Reflector->module[2] = 0, Reflector->module[3] = 0, Reflector->module[4] = 0;
	for (int i = 0; i < 5; i++)
	{
		if (Reflector->module[i] != 0) Reflector->module_charged[i] = ChargedMod;
		else Reflector->module_charged[i] = false;
	}
	Reflector->position = 375, Reflector->size = 375, Reflector->speed = 1, Reflector->age = -100, Reflector->effect = 0;
	if (Reflector->module[0] & 1) Reflector->angle = 0;
	else Reflector->angle = 0.25;
	return Reflector;
}
struct Power_Reflector* ReflectorApply(struct Power_Reflector* Reflector, int Count)			// 반사판 적용
{
	Reflector->position = ReflectorHead->position;
	Reflector->size = ReflectorHead->size;
	Reflector->speed = ReflectorHead->speed;
	Reflector->age = ReflectorHead->age;
	Reflector->effect = ReflectorHead->effect;
	for (int i = 0; i < 5; i++)
	{
		Reflector->module[i] = ReflectorHead->module[i];
		Reflector->module_charged[i] = ReflectorHead->module_charged[i];
	}
	if (Count == 0) Reflector->module_charged[0] = true;
	Reflector->angle = ReflectorHead->angle + Count / (Reflector->module[0] + 1.0);
	return Reflector;
}
//--------------------------------------------------------------------------------------------------------------//