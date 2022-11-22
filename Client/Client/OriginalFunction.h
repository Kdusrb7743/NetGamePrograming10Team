#pragma once
#define _USE_MATH_DEFINES					// 파이값 불러오기 용
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <tchar.h>
#include <atlImage.h>
#include <math.h>

enum PacketType
{
	NONE,
	LOBBY,
	MAIN,
	END
};

static HDC hdc, memdc;
static HBITMAP hBitmap;
static HWND hWnd;
static PAINTSTRUCT ps;

//--------------------------------------------------------------------------------------------------------------//
// 전역변수
float window_size = 0.96;
bool WASD = false, RestartPressure = false, debug = true, keyboard = true; // ChargedMod = false,

int ReactorEffect = 0;					// 경기장 이펙트 출력용 변수
float Score = 0;						// 스코어
//----------------------------------------------------------------------- 체크 부분
// 애니메이션에 쓸 시간 부분이 필요 - 한 프레임당
int Time = 0;

//방향키
int Reflector1Left = 0x25;				//0x25 - 왼쪽 방향키
int Reflector1Right = 0X27;
int Reflector1Up = 0X26;
int Reflector1Down = 0X28;

int Player1RGB[3] = { 255, 255, 0 };

double window_half = window_size * 0.5;
double window_size_x = 2000 * window_size, window_size_y = 1125 * window_size, Pibot_x = window_size_x * 0.5, Pibot_y = window_size_y * 0.5;

double Reactor_size = 1000, Rail_size = 375, Orb_size = 50;
double Controllroom_size_x = 3000, Reflector_size_x = 375;
double Controllroom_size_y = 2000, Reflector_size_y = 115;

int Reactor_window = int(Reactor_size * window_size), Rail_window = int(Rail_size * window_size), Orb_window = int(Orb_size * window_size);
int Controllroom_window_x = int(Controllroom_size_x * window_size), Reflector_window_x = int(Reflector_size_x * window_size);
int Controllroom_window_y = int(Controllroom_size_y * window_size), Reflector_window_y = int(Reflector_size_y * window_size);

int Reactor_half = int(Reactor_size * window_half), Rail_half = int(Rail_size * window_half), Orb_half = int(Orb_size * window_half);
int Controllroom_half_x = int(Controllroom_size_x * window_half), Reflector_half_x = int(Reflector_size_x * window_half);
int Controllroom_half_y = int(Controllroom_size_y * window_half), Reflector_half_y = int(Reflector_size_y * window_half);
//--------------------------------------------------------------------------------------------------------------//
CImage ReactorImg, Reactor_EffectImg, ReflectorImg, Reflector_EffectImg, OrbImg, PressureImg, CherenkovImg;
CImage Reactor_RailImg, Reflector_ColorImg, Reflector_LightImg, Reflector_ColorChargeImg, Reflector_LightChargeImg, Reflector_ColorOffImg, Reflector_LightOffImg;
CImage Reflector_Mask_Img, Reflector_Effect_Mask_Img, Reflector_Color_Mask_Img, Reflector_Light_Mask_Img;
CImage Button_PressureImg, Button_DialImg, Button_ValveImg, Button_OrbImg, Cherenkov_LeverImg, TempertureImg, DoorImg;
CImage Pressure_Mask_Img, Cherenkov_Mask_Img, Button_Valve_Mask_Img, Button_Dial_Mask_Img, Temperture_Mask_Img;
CImage Reflector_Module1_Img, Reflector_Module2_Img, Reflector_Module3_Img, Reflector_Module4_Img, Reflector_Module5_Img;
CImage Reflector_Module1_Mask_Img, Reflector_Module2_Mask_Img, Reflector_Module3_Mask_Img, Reflector_Module4_Mask_Img, Reflector_Module5_Mask_Img;
//--------------------------------------------------------------------------------------------------------------//
struct Power_Effect
{
	float x, y, score;
	int age;									// 얼마나 이펙트 살아있는지
	struct Power_Effect* next;
};

struct Power_Orb
{
	int effect, effect_count;
	float x, y, size;
	float afterx[25], aftery[25];
	struct Power_Orb* next;
};

struct Power_Reflector
{
	float angle, position, size, speed;
	int module[5], age, effect;
	struct Power_Reflector* next;
};

//Power_Effect	EffectHead;
//Power_Orb		OrbHead;
//Power_Reflector ReflectorHead;
static struct Power_Orb* OrbHead;
static struct Power_Reflector* ReflectorHead;
struct Power_Effect* EffectHead = (Power_Effect*)malloc(sizeof(struct Power_Effect));

// 서버에서 충돌이 일어나면 그 인자값을 가져가서 충돌이 일어났다고 클라에게 보내야 하나 보내지 않으면 점수만 보내줌
// 따라서 충돌 이펙트는 하나도 일어나지 않을 것으로 보임 

// 이 함수는 필요 없어 질 수 있음
void CreateEffect(struct Power_Effect* Effect, double x, double y, double Score) // 충돌 시 점수 이펙트 생성
{
	if (Effect->next == EffectHead)
	{
		Effect->next = new Power_Effect;
		Effect->next->x = x;
		Effect->next->y = y;
		Effect->next->score = Score;
		Effect->next->age = Time + 100;						// ----------------11.04  이펙트 진행에 시간이 관여
		Effect->next->next = EffectHead;
		return;
	}
	else CreateEffect(Effect->next, x, y, Score);
}

void RemoveEffect(struct Power_Effect* NextEffect, struct Power_Effect* Effect) // 이펙트 제거
{
	Effect->next = Effect->next->next;
	delete NextEffect;
	return;
}

void EffectPrint(struct Power_Effect* Effect) // 이펙트 출력
{
	if (Effect->next != EffectHead)
	{
		if (Effect->next->age == Time || Time < 1)			// 이펙트 진행에 시간이 관여----------------11.04
		{
			RemoveEffect(Effect->next, Effect);
		}
		else
		{
			TCHAR lpOut[100];
			HFONT hFont, oldFont;
			int Age = 100 - Effect->next->age + Time;
			hFont = CreateFont((int)(100 * window_size), 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"PowerIngElectric");
			oldFont = (HFONT)SelectObject(memdc, hFont);
			if (Effect->next->score > 1000)
				SetTextColor(memdc, RGB(255, 0, 255));
			else if (Effect->next->score > 500)
				SetTextColor(memdc, RGB(0, 255, 255));
			else if (Effect->next->score > 250)
				SetTextColor(memdc, RGB(255, 255, 0));
			else
				SetTextColor(memdc, RGB(255, 255, 255));
			swprintf_s(lpOut, 100, L"+\\%.0f ", Effect->next->score);
			TextOut(memdc, (int)(Pibot_x + Effect->next->x * window_size), (int)(Pibot_y + (Effect->next->y - Age * 2.0) * window_size), lpOut, lstrlen(lpOut));
			SelectObject(memdc, oldFont);
			DeleteObject(hFont);
		}
		EffectPrint(Effect->next);
	}
	else return;
}

void DisplayLoad() // 이미지들을 출력하고 png 파일들에게 색상 일치를 적용하는 함수
{
	ReactorImg.Load(TEXT("Img\\Reactor.png"));
	Reactor_EffectImg.Load(TEXT("Img\\Reactor_Effect.png"));
	OrbImg.Load(TEXT("Img\\Orb.png"));

	ReflectorImg.Load(TEXT("Img\\Reflector.png"));
	Reflector_EffectImg.Load(TEXT("Img\\Reflector_Effect.png"));

	Reflector_Mask_Img.Load(TEXT("Img\\Reflector_Mask.bmp"));
	Reflector_Effect_Mask_Img.Load(TEXT("Img\\Reflector_Effect_Mask.bmp"));
	Reflector_Color_Mask_Img.Load(TEXT("Img\\Reflector_Color_Mask.bmp"));
	Reflector_Light_Mask_Img.Load(TEXT("Img\\Reflector_Light_Mask.bmp"));

	DoorImg.Load(TEXT("Img\\Door.png"));


	for (int i = 0; i < Reactor_EffectImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reactor_EffectImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reactor_EffectImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] * ptr[3]) + 127) / 255;
			ptr[1] = ((ptr[1] * ptr[3]) + 127) / 255;
			ptr[2] = ((ptr[2] * ptr[3]) + 127) / 255;
		}
	}
	for (int i = 0; i < OrbImg.GetWidth(); i++)
	{
		for (int j = 0; j < OrbImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)OrbImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] * ptr[3]) + 127) / 255;
			ptr[1] = ((ptr[1] * ptr[3]) + 127) / 255;
			ptr[2] = ((ptr[2] * ptr[3]) + 127) / 255;
		}
	}
}

void DisplayWindow() // 윈도우 출력
{
	window_half = window_size * 0.5;
	window_size_x = 2000 * window_size, window_size_y = 1050 * window_size, Pibot_x = window_size_x * 0.5, Pibot_y = window_size_y * 0.5;

	Reactor_size = 1000, Rail_size = 375, Orb_size = 50;
	Controllroom_size_x = 3000, Reflector_size_x = 375;
	Controllroom_size_y = 2000, Reflector_size_y = 115;

	Reactor_window = int(Reactor_size * window_size), Rail_window = int(Rail_size * window_size), Orb_window = int(Orb_size * window_size);
	Controllroom_window_x = int(Controllroom_size_x * window_size), Reflector_window_x = int(Reflector_size_x * window_size);
	Controllroom_window_y = int(Controllroom_size_y * window_size), Reflector_window_y = int(Reflector_size_y * window_size);

	Reactor_half = int(Reactor_size * window_half), Rail_half = int(Rail_size * window_half), Orb_half = int(Orb_size * window_half);
	Controllroom_half_x = int(Controllroom_size_x * window_half), Reflector_half_x = int(Reflector_size_x * window_half);
	Controllroom_half_y = int(Controllroom_size_y * window_half), Reflector_half_y = int(Reflector_size_y * window_half);
}

void DisplayColorApply() // 색상이 있는 이미지들에 색상값을 부여하는 함수
{
	Reactor_RailImg.Load(TEXT("Reactor_Rail.png"));
	Reflector_ColorImg.Load(TEXT("Reflector_Color.png"));
	Reflector_LightImg.Load(TEXT("Reflector_Color.png"));
	Reflector_ColorChargeImg.Load(TEXT("Reflector_Color.png"));
	Reflector_LightChargeImg.Load(TEXT("Reflector_Color.png"));
	Reflector_ColorOffImg.Load(TEXT("Reflector_Color.png"));
	Reflector_LightOffImg.Load(TEXT("Reflector_Color.png"));

	for (int i = 0; i < Reactor_RailImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reactor_RailImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reactor_RailImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 255 + Player1RGB[2]) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 255 + Player1RGB[1]) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 255 + Player1RGB[0]) * ptr[3] + 127) / 255;
		}
	}
	for (int i = 0; i < Reflector_LightImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reflector_LightImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reflector_LightImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 255 + Player1RGB[2]) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 255 + Player1RGB[1]) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 255 + Player1RGB[0]) * ptr[3] + 127) / 255;
		}
	}
	for (int i = 0; i < Reflector_ColorImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reflector_ColorImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reflector_ColorImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 255 + Player1RGB[2] * 0.9) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 255 + Player1RGB[1] * 0.9) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 255 + Player1RGB[0] * 0.9) * ptr[3] + 127) / 255;
		}
	}
	for (int i = 0; i < Reflector_LightChargeImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reflector_LightChargeImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reflector_LightChargeImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 26 + Player1RGB[2] * 0.1) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 26 + Player1RGB[1] * 0.1) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 26 + Player1RGB[0] * 0.1) * ptr[3] + 127) / 255;
		}
	}
	for (int i = 0; i < Reflector_ColorChargeImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reflector_ColorChargeImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reflector_ColorChargeImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 255 + Player1RGB[2]) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 255 + Player1RGB[1]) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 255 + Player1RGB[0]) * ptr[3] + 127) / 255;
		}
	}
	for (int i = 0; i < Reflector_LightOffImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reflector_LightOffImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reflector_LightOffImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 196 + Player1RGB[2] * 0.5) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 196 + Player1RGB[1] * 0.5) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 196 + Player1RGB[0] * 0.5) * ptr[3] + 127) / 255;
		}
	}
	for (int i = 0; i < Reflector_ColorOffImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reflector_ColorOffImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reflector_ColorOffImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 196 + Player1RGB[2] * 0.45) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 196 + Player1RGB[1] * 0.45) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 196 + Player1RGB[0] * 0.45) * ptr[3] + 127) / 255;
		}
	}
}

void Display() // 출력 전반을 관리하는 함수
{
	memdc = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, (int)window_size_x, (int)window_size_y);

	SelectObject(memdc, hBitmap);
	SetStretchBltMode(memdc, COLORONCOLOR);

	ReactorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
	Reactor_EffectImg.Draw(memdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size * (ReactorEffect % 6), Reactor_size * (int)(ReactorEffect / 6), Reactor_size, Reactor_size);

	DisplayOrb(OrbHead);
	DisplayReflector(ReflectorHead);

	SetBkMode(memdc, TRANSPARENT);
	UIScore();
	EffectPrint(EffectHead);
	if (debug == true)	UIDebugInfo();

	BitBlt(hdc, 0, 0, (int)window_size_x, (int)window_size_y, memdc, 0, 0, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteDC(memdc);
}

void DisplayOrb(struct Power_Orb* Orb) // 오브들을 출력하는 함수
{
	if (Orb->next != OrbHead)
	{
		for (int i = Orb->next->effect_count; i > 0; i--) OrbImg.AlphaBlend(memdc, int(Pibot_x + (Orb->next->afterx[i] - Orb->next->size) * window_size), int(Pibot_y + (Orb->next->aftery[i] - Orb->next->size) * window_size), int(Orb->next->size * 2 * window_size), int(Orb->next->size * 2 * window_size), Orb_size * (4 * 0 + 10 * 1 + 2), Orb_size * 2, Orb_size, Orb_size, 1.125 * (Orb->next->effect_count - i));
		for (int i = Orb->next->effect_count; i > 0; i--) OrbImg.AlphaBlend(memdc, int(Pibot_x + (Orb->next->afterx[i] - Orb->next->size) * window_size), int(Pibot_y + (Orb->next->aftery[i] - Orb->next->size) * window_size), int(Orb->next->size * 2 * window_size), int(Orb->next->size * 2 * window_size), Orb_size * (4 * 0 + 10 * 1 + 3), Orb_size * 2, Orb_size, Orb_size, 1.125 * (Orb->next->effect_count - i));
		for (int i = Orb->next->effect_count; i > 0; i--) OrbImg.AlphaBlend(memdc, int(Pibot_x + (Orb->next->afterx[i] - Orb->next->size) * window_size), int(Pibot_y + (Orb->next->aftery[i] - Orb->next->size) * window_size), int(Orb->next->size * 2 * window_size), int(Orb->next->size * 2 * window_size), Orb_size * (4 * 0 + 10 * 1 + 4), Orb_size * 2, Orb_size, Orb_size, 1.125 * (Orb->next->effect_count - i));
		for (int i = Orb->next->effect_count; i > 0; i--) OrbImg.AlphaBlend(memdc, int(Pibot_x + (Orb->next->afterx[i] - Orb->next->size) * window_size), int(Pibot_y + (Orb->next->aftery[i] - Orb->next->size) * window_size), int(Orb->next->size * 2 * window_size), int(Orb->next->size * 2 * window_size), Orb_size * (4 * 0 + 10 * 1 + 5), Orb_size * 2, Orb_size, Orb_size, 1.125 * (Orb->next->effect_count - i));
		OrbImg.Draw(memdc, int(Pibot_x + (Orb->next->x - Orb->next->size) * window_size), int(Pibot_y + (Orb->next->y - Orb->next->size) * window_size), int(Orb->next->size * 2 * window_size), int(Orb->next->size * 2 * window_size), Orb_size * (0 + 10 * 1), Orb_size * 2, Orb_size, Orb_size);
		DisplayOrb(Orb->next);
	}
	else return;
}

void DisplayReflector(struct Power_Reflector* Reflector) // 리플렉터(반사판)들을 출력하는 함수
{
	if (Reflector->next != ReflectorHead)
	{
		POINT Reflector_Point[3] = {
		ReflectorPaint1(Reflector->next, 0),
		ReflectorPaint2(Reflector->next, 0),
		ReflectorPaint3(Reflector->next, 0)
		};
		ReflectorImg.PlgBlt(memdc, Reflector_Point, 0, 115, 375, 115, Reflector_Mask_Img, 0, 0);

		if (Reflector->next->age < -100)
		{
			Reflector_ColorImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
			Reflector_LightImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);
		}
		else if (Reflector->next->age > 0)
		{
			Reflector_ColorOffImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
			Reflector_LightOffImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);
		}
		else
		{
			Reflector_ColorChargeImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
			Reflector_LightChargeImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);
		}

		if (Reflector->next->effect > 0)
		{
			POINT Reflector_Effect_Point[3] = {
			ReflectorPaint1(Reflector->next, 67.5),
			ReflectorPaint2(Reflector->next, 67.5),
			ReflectorPaint3(Reflector->next, 67.5)
			};
			Reflector_EffectImg.PlgBlt(memdc, Reflector_Effect_Point, 375 * 3, (300 * (int)(Reflector->next->effect / 100.0)), 375, 300, Reflector_Effect_Mask_Img, 375 * (Reflector->next->effect % 100), 0);
			// Reflector_EffectImg.PlgBlt(memdc, Reflector_Effect_Point, 375 * (Reflector->next->effect % 100), (300 * (int)(Reflector->next->effect / 100.0)), 375, 300, Reflector_Effect_Mask_Img, 375 * (Reflector->next->effect % 100), 0);
		}
		DisplayReflector(Reflector->next);
	}
	else return;
}

//--------------------------------------------------------------------------------------------------------------//
void UIMenu(bool Start, bool Module, bool Option, bool Quit, bool Esc) // 로비씬 UI를 출력하는 함수
{
	HFONT hFont, oldFont;
	hFont = CreateFont((int)(300 * window_size), 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"PowerIngElectric");
	oldFont = (HFONT)SelectObject(memdc, hFont);

	SetTextColor(memdc, RGB(0, 255, 0));
	TextOut(memdc, int(Pibot_x - 800 * window_size), int(Pibot_y - 400 * window_size), L"Power Ing\\", lstrlen(L"Power Ing\\"));

	hFont = CreateFont((int)(150 * window_size), 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"PowerIngElectric");
	oldFont = (HFONT)SelectObject(memdc, hFont);

	UIButton(-650, -125, 0, 255, 0, 0, 255, 255, Start, L"Start!");
	UIButton(-650, 25, 0, 255, 0, 0, 255, 255, Module, L"Module");
	UIButton(-650, 175, 0, 255, 0, 0, 255, 255, Option, L"Options");
	UIButton(-650, 325, 255, 255, 0, 255, 0, 0, Quit, L"Quit");

	SelectObject(memdc, oldFont);
	DeleteObject(hFont);
}

void UIButton(int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30]) // 메인메뉴 버튼을 출력하는 함수 , 선택 여부에 따라 위치 변경
{
	TCHAR lpOut[100];
	if (Seleted)
	{
		SetTextColor(memdc, RGB(SR, SG, SB));
		swprintf_s(lpOut, 100, L"  \\%s", String);
	}
	else
	{
		SetTextColor(memdc, RGB(R, G, B));
		swprintf_s(lpOut, 100, L"%s", String);
	}
	TextOut(memdc, int(Pibot_x + x * window_size), int(Pibot_y + y * window_size), lpOut, lstrlen(lpOut));
}

bool UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse) // 메인메뉴 버튼이 선택되었는지를 판단하는 함수
{
	return (Pibot_x + x * window_size < Mouse.x&& Mouse.x < Pibot_x + (x + sizex) * window_size && Pibot_y + y * window_size < Mouse.y&& Mouse.y < Pibot_y + (y + sizey) * window_size);
}

void UIEndMessage() // 게임오버 시 메세지를 출력하는 함수			// ----------------11.04 이부분 3명의 클라 모두 출력 수정
{
	TCHAR lpOut[100];
	HFONT hFont, oldFont;
	hFont = CreateFont((int)(200 * window_size), 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"PowerIngElectric");
	oldFont = (HFONT)SelectObject(memdc, hFont);

	if (Score < 1000)
	{
		SetTextColor(memdc, RGB(0, 255, 0));
		swprintf_s(lpOut, 100, L"Score:\\%.0f ", Score);
	}
	else if (Score < 1000000)
	{
		if (Score < 10000) SetTextColor(memdc, RGB(255, 255, 255));
		else if (Score < 100000)  SetTextColor(memdc, RGB(255, 255, 0));
		else if (Score < 500000)  SetTextColor(memdc, RGB(0, 255, 255));
		else  SetTextColor(memdc, RGB(255, 0, 255));
		swprintf_s(lpOut, 100, L"Score:\\%.3gK", Score / 1000);
	}
	else if (Score < 1000000000)
	{
		SetTextColor(memdc, RGB(255, 0, 0));
		swprintf_s(lpOut, 100, L"Score:\\%.3gM", Score / 1000000);
	}
	else if (Score < 1000000000000)
	{
		SetTextColor(memdc, RGB(0, 0, 255));
		swprintf_s(lpOut, 100, L"Score:\\%.3gB", Score / 1000000000);
	}
	else
	{
		SetTextColor(memdc, RGB(255, 192, 0));
		swprintf_s(lpOut, 100, L"Score:\\MAX\\");
	}

	TextOut(memdc, int(Pibot_x - 400 * window_size), int(Pibot_y + 70 * window_size), lpOut, lstrlen(lpOut));
	TextOut(memdc, int(Pibot_x - 400 * window_size), int(Pibot_y - 210 * window_size), L"Game Over", lstrlen(L"Game Over"));

	SelectObject(memdc, oldFont);
	DeleteObject(hFont);
}

void UIScore() // 현재 자신의 점수 표기 함수
{
	TCHAR lpOut[100];
	HFONT hFont, oldFont;
	hFont = CreateFont((int)(100 * window_size), 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"PowerIngElectric");
	oldFont = (HFONT)SelectObject(memdc, hFont);
	SetTextColor(memdc, RGB(0, 255, 0));
	if (Score < 1000) swprintf_s(lpOut, 100, L"\\%.0f ", Score);
	else if (Score < 1000000) swprintf_s(lpOut, 100, L"\\%.3gK", Score / 1000);
	else if (Score < 1000000000) swprintf_s(lpOut, 100, L"\\%.3gM", Score / 1000000);
	else if (Score < 1000000000000) swprintf_s(lpOut, 100, L"\\%.3gB", Score / 1000000000);
	else swprintf_s(lpOut, 100, L"\\MAX\\");
	TextOut(memdc, 1600 * window_size, 525 * window_size, lpOut, lstrlen(lpOut));
	TextOut(memdc, 1600 * window_size, 450 * window_size, L"Power", 5);

	SelectObject(memdc, oldFont);
	DeleteObject(hFont);
}

void UIDebugInfo() // 디버그용
{
	TCHAR lpOut[100];
	SetTextColor(memdc, RGB(0, 0, 0));
	//swprintf_s(lpOut, 100, L"Orb : %g ", OrbHead->next->angle);
	//TextOut(memdc, 0, 0, lpOut, lstrlen(lpOut));
	swprintf_s(lpOut, 100, L"x : %g ", OrbHead->next->x);
	TextOut(memdc, 0, 12, lpOut, lstrlen(lpOut));
	swprintf_s(lpOut, 100, L"y : %g ", OrbHead->next->y);
	TextOut(memdc, 0, 25, lpOut, lstrlen(lpOut));
	//swprintf_s(lpOut, 100, L"relative : %g ", AnglePosition(OrbHead->next->x, OrbHead->next->y));
	//TextOut(memdc, 0, 37, lpOut, lstrlen(lpOut));
	swprintf_s(lpOut, 100, L"reflector : %g ", ReflectorHead->next->angle);
	TextOut(memdc, 0, 50, lpOut, lstrlen(lpOut));
	swprintf_s(lpOut, 100, L"speed : %g ", ReflectorHead->next->speed);
	TextOut(memdc, 0, 62, lpOut, lstrlen(lpOut));
	swprintf_s(lpOut, 100, L"age : %d ", ReflectorHead->next->age);
	TextOut(memdc, 0, 75, lpOut, lstrlen(lpOut));
	swprintf_s(lpOut, 100, L"position : %g ", ReflectorHead->next->position);
	TextOut(memdc, 0, 87, lpOut, lstrlen(lpOut));
	swprintf_s(lpOut, 100, L"distance : %g ", sqrt(OrbHead->next->x * OrbHead->next->x + OrbHead->next->y * OrbHead->next->y));
	TextOut(memdc, 0, 162, lpOut, lstrlen(lpOut));
	swprintf_s(lpOut, 100, L"effect : %g ", OrbHead->next->afterx[0]);
	TextOut(memdc, 0, 175, lpOut, lstrlen(lpOut));

	return;
}

//--------------------------------------------------------------------------------------------------------------//

const float M_TU = M_PI * 2;

float PointRotationX(float x, float y, float angle)					// 이미지 회전 출력을 위한 X좌표 계산
{
	return x * cos(M_TU * angle) - y * sin(M_TU * angle);
}
float PointRotationY(float x, float y, float angle)					// 이미지 회전 출력을 위한 y좌표 계산
{
	return x * sin(M_TU * angle) + y * cos(M_TU * angle);
}

//--------------------------------------------------------------------------------------------------------------//
POINT RotatePaint1(double x, double y, double sizex, double sizey, double angle) // 이미지 회전 함수 - 1번 좌표
{
	return{ (long)(Pibot_x + x * window_size + PointRotationX(-sizex * window_half, -sizey * window_half, angle)), (long)(Pibot_y + y * window_size + PointRotationY(-sizex * window_half, -sizey * window_half, angle)) };
}
POINT RotatePaint2(double x, double y, double sizex, double sizey, double angle) // 이미지 회전 함수 - 2번 좌표
{
	return{ (long)(Pibot_x + x * window_size + PointRotationX(sizex * window_half, -sizey * window_half, angle)), (long)(Pibot_y + y * window_size + PointRotationY(sizex * window_half, -sizey * window_half, angle)) };
}
POINT RotatePaint3(double x, double y, double sizex, double sizey, double angle) // 이미지 회전 함수 - 3번 좌표
{
	return{ (long)(Pibot_x + x * window_size + PointRotationX(-sizex * window_half, sizey * window_half, angle)), (long)(Pibot_y + y * window_size + PointRotationY(-sizex * window_half, sizey * window_half, angle)) };
}
//--------------------------------------------------------------------------------------------------------------//
POINT ReflectorPaint1(struct Power_Reflector* Reflector, double Vertical) // 리플렉터 회전 전용 함수 - 1번 좌표
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, Reflector_half_x, Reflector->angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, Reflector_half_x, Reflector->angle)) };
}
POINT ReflectorPaint2(struct Power_Reflector* Reflector, double Vertical) // 리플렉터 회전 전용 함수 - 2번 좌표
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, -Reflector_half_x, Reflector->angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, -Reflector_half_x, Reflector->angle)) };
}
POINT ReflectorPaint3(struct Power_Reflector* Reflector, double Vertical) // 리플렉터 회전 전용 함수 - 3번 좌표
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) + Reflector_half_y + (25 + Vertical) * window_size, Reflector_half_x, Reflector->angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) + Reflector_half_y + (25 + Vertical) * window_size, Reflector_half_x, Reflector->angle)) };
}


//--------------------------------------------------------------------------------------------------------------//
// 문 애니메이션			렌더에 들어갈 함수   -   시간은 다시 정의
void DoorAnimation() {		// 열리거나 닫히는 애니메이션
	DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y,
		Controllroom_size_x * (Time % 5), Controllroom_size_y * (int)(Time / 5), Controllroom_size_x, Controllroom_size_y); // 3000 * (PreTime % 5), 2000 * (int)(PreTime / 5)
}

void DoorIdle() {
	DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
}


//--------------------------------------------------------------------------------------------------------------//
//		Reader() 함수
// 랜더 함수에서는 로비씬 메인씬 엔드씬으로 해당 씬에 알맞는 그림들을 그려줌
// 메인씬에서만 반사판, 공 등 객체들을 그려주고, 나머지 씬에서는 문으로 닫힌 상태만 보여주게 된다.
// 
//--------------------------------------------------------------------------------------------------------------//

void Render() {						// -----------------------------------------------렌더함수
	//더블버퍼용
	hdc = BeginPaint(hWnd, &ps);
	memdc = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, (int)window_size_x, (int)window_size_y);

	SelectObject(memdc, hBitmap);
	SetStretchBltMode(memdc, COLORONCOLOR);
	SetBkMode(memdc, TRANSPARENT);


	if (LOBBY)		// 현재 로비 씬이면
	{
		//닫힌 상태의 문
		DoorIdle();
	}
	else if (END)			// 엔드 씬이면
	{
		//닫힌 상태의 문
		DoorIdle();
		UIEndMessage();
	}
	else					// MAIN 씬이면
	{
		// 게임장과 레일 그리기
		ReactorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
		Reactor_RailImg.Draw(memdc, Pibot_x - 782 * window_half, Pibot_y - 782 * window_half, 782 * window_size, 782 * window_size, 0, 0, 782, 782);

		// 반사판그리기
		DisplayReflector(ReflectorHead);

		if (ReactorEffect < 12) Reactor_EffectImg.Draw(memdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size * (ReactorEffect % 6), Reactor_size * (int)(ReactorEffect / 6), Reactor_size, Reactor_size);
		else Reactor_EffectImg.Draw(memdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, 5000, 1000, Reactor_size, Reactor_size);

		DisplayOrb(OrbHead);						// 공 그리기

		// 디버그 출력, UI-점수, 이펙트 출력
		if (debug)	UIDebugInfo();

		UIScore();
		EffectPrint(EffectHead);

		// 문 애니메이션 씬			-- 여기서 열리는 애니메이션이 객체가 그려져 있는 앞에 덧씌워 진행된다.
		if (1) {					//로비씬에 막 왔을 경우
			DoorAnimation();
		}
	}
	BitBlt(hdc, 0, 0, (int)window_size_x, (int)window_size_y, memdc, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteDC(memdc);

	//-----------------------------------------------------------------------------------------------------------------------------//

	EndPaint(hWnd, &ps);
}
