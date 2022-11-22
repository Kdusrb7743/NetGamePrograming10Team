#pragma once
#define _USE_MATH_DEFINES					// ���̰� �ҷ����� ��
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
// ��������
float window_size = 0.96;
bool WASD = false, RestartPressure = false, debug = true, keyboard = true; // ChargedMod = false,

int ReactorEffect = 0;					// ����� ����Ʈ ��¿� ����
float Score = 0;						// ���ھ�
//----------------------------------------------------------------------- üũ �κ�
// �ִϸ��̼ǿ� �� �ð� �κ��� �ʿ� - �� �����Ӵ�
int Time = 0;

//����Ű
int Reflector1Left = 0x25;				//0x25 - ���� ����Ű
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
	int age;									// �󸶳� ����Ʈ ����ִ���
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

// �������� �浹�� �Ͼ�� �� ���ڰ��� �������� �浹�� �Ͼ�ٰ� Ŭ�󿡰� ������ �ϳ� ������ ������ ������ ������
// ���� �浹 ����Ʈ�� �ϳ��� �Ͼ�� ���� ������ ���� 

// �� �Լ��� �ʿ� ���� �� �� ����
void CreateEffect(struct Power_Effect* Effect, double x, double y, double Score) // �浹 �� ���� ����Ʈ ����
{
	if (Effect->next == EffectHead)
	{
		Effect->next = new Power_Effect;
		Effect->next->x = x;
		Effect->next->y = y;
		Effect->next->score = Score;
		Effect->next->age = Time + 100;						// ----------------11.04  ����Ʈ ���࿡ �ð��� ����
		Effect->next->next = EffectHead;
		return;
	}
	else CreateEffect(Effect->next, x, y, Score);
}

void RemoveEffect(struct Power_Effect* NextEffect, struct Power_Effect* Effect) // ����Ʈ ����
{
	Effect->next = Effect->next->next;
	delete NextEffect;
	return;
}

void EffectPrint(struct Power_Effect* Effect) // ����Ʈ ���
{
	if (Effect->next != EffectHead)
	{
		if (Effect->next->age == Time || Time < 1)			// ����Ʈ ���࿡ �ð��� ����----------------11.04
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

void DisplayLoad() // �̹������� ����ϰ� png ���ϵ鿡�� ���� ��ġ�� �����ϴ� �Լ�
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

void DisplayWindow() // ������ ���
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

void DisplayColorApply() // ������ �ִ� �̹����鿡 ������ �ο��ϴ� �Լ�
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

void Display() // ��� ������ �����ϴ� �Լ�
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

void DisplayOrb(struct Power_Orb* Orb) // ������� ����ϴ� �Լ�
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

void DisplayReflector(struct Power_Reflector* Reflector) // ���÷���(�ݻ���)���� ����ϴ� �Լ�
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
void UIMenu(bool Start, bool Module, bool Option, bool Quit, bool Esc) // �κ�� UI�� ����ϴ� �Լ�
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

void UIButton(int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30]) // ���θ޴� ��ư�� ����ϴ� �Լ� , ���� ���ο� ���� ��ġ ����
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

bool UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse) // ���θ޴� ��ư�� ���õǾ������� �Ǵ��ϴ� �Լ�
{
	return (Pibot_x + x * window_size < Mouse.x&& Mouse.x < Pibot_x + (x + sizex) * window_size && Pibot_y + y * window_size < Mouse.y&& Mouse.y < Pibot_y + (y + sizey) * window_size);
}

void UIEndMessage() // ���ӿ��� �� �޼����� ����ϴ� �Լ�			// ----------------11.04 �̺κ� 3���� Ŭ�� ��� ��� ����
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

void UIScore() // ���� �ڽ��� ���� ǥ�� �Լ�
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

void UIDebugInfo() // ����׿�
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

float PointRotationX(float x, float y, float angle)					// �̹��� ȸ�� ����� ���� X��ǥ ���
{
	return x * cos(M_TU * angle) - y * sin(M_TU * angle);
}
float PointRotationY(float x, float y, float angle)					// �̹��� ȸ�� ����� ���� y��ǥ ���
{
	return x * sin(M_TU * angle) + y * cos(M_TU * angle);
}

//--------------------------------------------------------------------------------------------------------------//
POINT RotatePaint1(double x, double y, double sizex, double sizey, double angle) // �̹��� ȸ�� �Լ� - 1�� ��ǥ
{
	return{ (long)(Pibot_x + x * window_size + PointRotationX(-sizex * window_half, -sizey * window_half, angle)), (long)(Pibot_y + y * window_size + PointRotationY(-sizex * window_half, -sizey * window_half, angle)) };
}
POINT RotatePaint2(double x, double y, double sizex, double sizey, double angle) // �̹��� ȸ�� �Լ� - 2�� ��ǥ
{
	return{ (long)(Pibot_x + x * window_size + PointRotationX(sizex * window_half, -sizey * window_half, angle)), (long)(Pibot_y + y * window_size + PointRotationY(sizex * window_half, -sizey * window_half, angle)) };
}
POINT RotatePaint3(double x, double y, double sizex, double sizey, double angle) // �̹��� ȸ�� �Լ� - 3�� ��ǥ
{
	return{ (long)(Pibot_x + x * window_size + PointRotationX(-sizex * window_half, sizey * window_half, angle)), (long)(Pibot_y + y * window_size + PointRotationY(-sizex * window_half, sizey * window_half, angle)) };
}
//--------------------------------------------------------------------------------------------------------------//
POINT ReflectorPaint1(struct Power_Reflector* Reflector, double Vertical) // ���÷��� ȸ�� ���� �Լ� - 1�� ��ǥ
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, Reflector_half_x, Reflector->angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, Reflector_half_x, Reflector->angle)) };
}
POINT ReflectorPaint2(struct Power_Reflector* Reflector, double Vertical) // ���÷��� ȸ�� ���� �Լ� - 2�� ��ǥ
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, -Reflector_half_x, Reflector->angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, -Reflector_half_x, Reflector->angle)) };
}
POINT ReflectorPaint3(struct Power_Reflector* Reflector, double Vertical) // ���÷��� ȸ�� ���� �Լ� - 3�� ��ǥ
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) + Reflector_half_y + (25 + Vertical) * window_size, Reflector_half_x, Reflector->angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector->position * Reflector->position - Reflector->size * Reflector->size * 0.25) + Reflector_half_y + (25 + Vertical) * window_size, Reflector_half_x, Reflector->angle)) };
}


//--------------------------------------------------------------------------------------------------------------//
// �� �ִϸ��̼�			������ �� �Լ�   -   �ð��� �ٽ� ����
void DoorAnimation() {		// �����ų� ������ �ִϸ��̼�
	DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y,
		Controllroom_size_x * (Time % 5), Controllroom_size_y * (int)(Time / 5), Controllroom_size_x, Controllroom_size_y); // 3000 * (PreTime % 5), 2000 * (int)(PreTime / 5)
}

void DoorIdle() {
	DoorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
}


//--------------------------------------------------------------------------------------------------------------//
//		Reader() �Լ�
// ���� �Լ������� �κ�� ���ξ� ��������� �ش� ���� �˸´� �׸����� �׷���
// ���ξ������� �ݻ���, �� �� ��ü���� �׷��ְ�, ������ �������� ������ ���� ���¸� �����ְ� �ȴ�.
// 
//--------------------------------------------------------------------------------------------------------------//

void Render() {						// -----------------------------------------------�����Լ�
	//������ۿ�
	hdc = BeginPaint(hWnd, &ps);
	memdc = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, (int)window_size_x, (int)window_size_y);

	SelectObject(memdc, hBitmap);
	SetStretchBltMode(memdc, COLORONCOLOR);
	SetBkMode(memdc, TRANSPARENT);


	if (LOBBY)		// ���� �κ� ���̸�
	{
		//���� ������ ��
		DoorIdle();
	}
	else if (END)			// ���� ���̸�
	{
		//���� ������ ��
		DoorIdle();
		UIEndMessage();
	}
	else					// MAIN ���̸�
	{
		// ������� ���� �׸���
		ReactorImg.Draw(memdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
		Reactor_RailImg.Draw(memdc, Pibot_x - 782 * window_half, Pibot_y - 782 * window_half, 782 * window_size, 782 * window_size, 0, 0, 782, 782);

		// �ݻ��Ǳ׸���
		DisplayReflector(ReflectorHead);

		if (ReactorEffect < 12) Reactor_EffectImg.Draw(memdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size * (ReactorEffect % 6), Reactor_size * (int)(ReactorEffect / 6), Reactor_size, Reactor_size);
		else Reactor_EffectImg.Draw(memdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, 5000, 1000, Reactor_size, Reactor_size);

		DisplayOrb(OrbHead);						// �� �׸���

		// ����� ���, UI-����, ����Ʈ ���
		if (debug)	UIDebugInfo();

		UIScore();
		EffectPrint(EffectHead);

		// �� �ִϸ��̼� ��			-- ���⼭ ������ �ִϸ��̼��� ��ü�� �׷��� �ִ� �տ� ������ ����ȴ�.
		if (1) {					//�κ���� �� ���� ���
			DoorAnimation();
		}
	}
	BitBlt(hdc, 0, 0, (int)window_size_x, (int)window_size_y, memdc, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteDC(memdc);

	//-----------------------------------------------------------------------------------------------------------------------------//

	EndPaint(hWnd, &ps);
}
