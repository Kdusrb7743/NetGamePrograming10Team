#include "stdafx.h"
#include "Renderer.h"


//-------------------------------------------------------------------------------------------------------------------
// 전역변수 선언
struct Power_Orb Orb = { 0.f, 0.f, 0.f, 0.f };

Power_Reflector Reflectors[3] = {
	{0.25f, 375.f, 375.f, 1.f},
	{0.583f, 375.f, 375.f, 1.f},
	{0.915f, 375.f, 375.f, 1.f}
};

float PScore[3] = {0.f, 0.f, 0.f};				// 3명의 플레이어 스코어

int Player1RGB[3] = { 255, 255, 0 };			// 색 정보
int Player2RGB[3] = { 255, 0, 0 };
int Player3RGB[3] = { 0, 255, 0 };
int RailRGB[3] = { 255, 255, 255 };

//윈도우 사이즈 변수
// float window_size = 0.96;					// 메인 사이즈

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

//이미지 변수
CImage ReactorImg, Reactor_EffectImg, ReflectorImg, Reflector_EffectImg, OrbImg, PressureImg, CherenkovImg;
CImage Reactor_RailImg, Reflector_ColorImg, Reflector_LightImg, Reflector_ColorChargeImg, Reflector_LightChargeImg, Reflector_ColorOffImg, Reflector_LightOffImg;
CImage Reflector_Mask_Img, Reflector_Effect_Mask_Img, Reflector_Color_Mask_Img, Reflector_Light_Mask_Img;
CImage Button_PressureImg, Button_DialImg, Button_ValveImg, Button_OrbImg, Cherenkov_LeverImg, TempertureImg, DoorImg;
CImage Pressure_Mask_Img, Cherenkov_Mask_Img, Button_Valve_Mask_Img, Button_Dial_Mask_Img, Temperture_Mask_Img;
CImage Reflector_Module1_Img, Reflector_Module2_Img, Reflector_Module3_Img, Reflector_Module4_Img, Reflector_Module5_Img;
CImage Reflector_Module1_Mask_Img, Reflector_Module2_Mask_Img, Reflector_Module3_Mask_Img, Reflector_Module4_Mask_Img, Reflector_Module5_Mask_Img;
CImage Reflector_ColorChargeImg2, Reflector_ColorChargeImg3;
//----------------------------------------------------------------------------------------------------------------


void Renderer::DisplayLoad()
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

void Renderer::DisplayColorApply()
{
	Reactor_RailImg.Load(TEXT("Img\\Reactor_Rail.png"));
	Reflector_ColorImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_LightImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_ColorChargeImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_ColorChargeImg2.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_ColorChargeImg3.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_LightChargeImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_ColorOffImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_LightOffImg.Load(TEXT("Img\\Reflector_Color.png"));

	for (int i = 0; i < Reactor_RailImg.GetWidth(); i++)
	{
		for (int j = 0; j < Reactor_RailImg.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reactor_RailImg.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 255 + RailRGB[2]) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 255 + RailRGB[1]) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 255 + RailRGB[0]) * ptr[3] + 127) / 255;
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
	for (int i = 0; i < Reflector_ColorChargeImg2.GetWidth(); i++)
	{
		for (int j = 0; j < Reflector_ColorChargeImg2.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reflector_ColorChargeImg2.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 255 + Player2RGB[2]) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 255 + Player2RGB[1]) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 255 + Player2RGB[0]) * ptr[3] + 127) / 255;
		}
	}
	for (int i = 0; i < Reflector_ColorChargeImg3.GetWidth(); i++)
	{
		for (int j = 0; j < Reflector_ColorChargeImg3.GetHeight(); j++)
		{
			BYTE* ptr = (BYTE*)Reflector_ColorChargeImg3.GetPixelAddress(i, j);
			ptr[0] = ((ptr[0] - 255 + Player3RGB[2]) * ptr[3] + 127) / 255;
			ptr[1] = ((ptr[1] - 255 + Player3RGB[1]) * ptr[3] + 127) / 255;
			ptr[2] = ((ptr[2] - 255 + Player3RGB[0]) * ptr[3] + 127) / 255;
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

void Renderer::DoorIdle(HDC hdc)
{
	DoorImg.Draw(hdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
}

void Renderer::UIMenu(HDC hdc, bool Start, bool Module, bool Option, bool Quit)
{
	HFONT hFont, oldFont;
	hFont = CreateFont((int)(300 * window_size), 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"PowerIngElectric");
	oldFont = (HFONT)SelectObject(hdc, hFont);

	SetTextColor(hdc, RGB(0, 255, 0));
	TextOut(hdc, int(Pibot_x - 800 * window_size), int(Pibot_y - 400 * window_size), L"Power Ing\\", lstrlen(L"Power Ing\\"));

	hFont = CreateFont((int)(150 * window_size), 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"PowerIngElectric");
	oldFont = (HFONT)SelectObject(hdc, hFont);

	UIButton(hdc, -650, -125, 0, 255, 0, 0, 255, 255, Start, L"Ready!");
	UIButton(hdc, -650, 25, 0, 255, 0, 0, 255, 255, Module, L"Module");
	UIButton(hdc, -650, 175, 0, 255, 0, 0, 255, 255, Option, L"Options");
	UIButton(hdc, -650, 325, 255, 255, 0, 255, 0, 0, Quit, L"Quit");

	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}

void Renderer::UIButton(HDC hdc, int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30])
{
	TCHAR lpOut[100];
	if (Seleted)
	{
		SetTextColor(hdc, RGB(SR, SG, SB));
		swprintf_s(lpOut, 100, L"  \\%s", String);
	}
	else
	{
		SetTextColor(hdc, RGB(R, G, B));
		swprintf_s(lpOut, 100, L"%s", String);
	}
	TextOut(hdc, int(Pibot_x + x * window_size), int(Pibot_y + y * window_size), lpOut, lstrlen(lpOut));
}

void Renderer::ReactorDraw(HDC hdc)
{
	ReactorImg.Draw(hdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
	Reactor_RailImg.Draw(hdc, Pibot_x - 782 * window_half, Pibot_y - 782 * window_half, 782 * window_size, 782 * window_size, 0, 0, 782, 782);
}

void Renderer::OrbDraw(HDC hdc)
{

}

bool Renderer::UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse)
{
	return (Pibot_x + x * window_size < Mouse.x&& Mouse.x < Pibot_x + (x + sizex) * window_size && Pibot_y + y * window_size < Mouse.y&& Mouse.y < Pibot_y + (y + sizey) * window_size);
}


void Renderer::DoorAnimation(HDC hdc, int Time)
{
	DoorImg.Draw(hdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y,
		Controllroom_window_x, Controllroom_window_y, Controllroom_size_x * (Time % 5),
		Controllroom_size_y * (int)(Time / 5), Controllroom_size_x, Controllroom_size_y);
}

void Renderer::UIScore(HDC hdc) // 현재 자신의 점수 표기 함수
{
	TCHAR lpOut[100];
	HFONT hFont, oldFont;
	hFont = CreateFont((int)(100 * window_size), 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"PowerIngElectric");
	oldFont = (HFONT)SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 255, 0));
	if (PScore[0] < 1000) swprintf_s(lpOut, 100, L"\\%.0f ", PScore[0]);
	else if (PScore[0] < 1000000) swprintf_s(lpOut, 100, L"\\%.3gK", PScore[0] / 1000);
	else if (PScore[0] < 1000000000) swprintf_s(lpOut, 100, L"\\%.3gM", PScore[0] / 1000000);
	else if (PScore[0] < 1000000000000) swprintf_s(lpOut, 100, L"\\%.3gB", PScore[0] / 1000000000);
	else swprintf_s(lpOut, 100, L"\\MAX\\");
	TextOut(hdc, 1600 * window_size, 525 * window_size, lpOut, lstrlen(lpOut));
	TextOut(hdc, 1600 * window_size, 450 * window_size, L"Power", 5);

	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}

void Renderer::DisplayReflector(HDC hdc) // 리플렉터(반사판)들을 출력하는 함수
{
	// 1플레이어 패널
	POINT Reflector_Point0[3] = {
	ReflectorPaint1(Reflectors[0], 0),
	ReflectorPaint2(Reflectors[0], 0),
	ReflectorPaint3(Reflectors[0], 0)
	};
	ReflectorImg.PlgBlt(hdc, Reflector_Point0, 0, 0, 375, 115, Reflector_Mask_Img, 0, 0);
	Reflector_ColorChargeImg.PlgBlt(hdc, Reflector_Point0, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
	Reflector_LightChargeImg.PlgBlt(hdc, Reflector_Point0, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);

	// 2플레이어 패널
	POINT Reflector_Point1[3] = {
	ReflectorPaint1(Reflectors[1], 0),
	ReflectorPaint2(Reflectors[1], 0),
	ReflectorPaint3(Reflectors[1], 0)
	};
	ReflectorImg.PlgBlt(hdc, Reflector_Point1, 0, 0, 375, 115, Reflector_Mask_Img, 0, 0);
	Reflector_ColorChargeImg2.PlgBlt(hdc, Reflector_Point1, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
	Reflector_LightChargeImg.PlgBlt(hdc, Reflector_Point1, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);

	// 3플레이어 패널
	POINT Reflector_Point2[3] = {
	ReflectorPaint1(Reflectors[2], 0),
	ReflectorPaint2(Reflectors[2], 0),
	ReflectorPaint3(Reflectors[2], 0)
	};
	ReflectorImg.PlgBlt(hdc, Reflector_Point2, 0, 0, 375, 115, Reflector_Mask_Img, 0, 0);
	Reflector_ColorChargeImg3.PlgBlt(hdc, Reflector_Point2, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
	Reflector_LightChargeImg.PlgBlt(hdc, Reflector_Point2, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);

	// 이펙트 후순위 변경
}

void Renderer::DisplayOrb(HDC hdc) // 오브들을 출력하는 함수
{
	// 이펙트 후순위 변경

	OrbImg.Draw(hdc, int(Pibot_x + (Orb.x - Orb.size) * window_size), int(Pibot_y + (Orb.y - Orb.size) * window_size), int(Orb.size * 2 * window_size), int(Orb.size * 2 * window_size), Orb_size * (0 + 10 * 1), Orb_size * 2, Orb_size, Orb_size);

}

float Renderer::PointRotationX(float x, float y, float angle)					// 이미지 회전 출력을 위한 X좌표 계산
{
	return x * cos(M_TU * angle) - y * sin(M_TU * angle);
}
float Renderer::PointRotationY(float x, float y, float angle)					// 이미지 회전 출력을 위한 y좌표 계산
{
	return x * sin(M_TU * angle) + y * cos(M_TU * angle);
}

POINT Renderer::ReflectorPaint1(struct Power_Reflector Reflector, double Vertical) // 리플렉터 회전 전용 함수 - 1번 좌표
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector.position * Reflector.position - Reflector.size * Reflector.size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, Reflector_half_x, Reflector.angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector.position * Reflector.position - Reflector.size * Reflector.size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, Reflector_half_x, Reflector.angle))};
}
POINT Renderer::ReflectorPaint2(struct Power_Reflector Reflector, double Vertical) // 리플렉터 회전 전용 함수 - 2번 좌표
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector.position * Reflector.position - Reflector.size * Reflector.size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, -Reflector_half_x, Reflector.angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector.position * Reflector.position - Reflector.size * Reflector.size * 0.25) - Reflector_half_y + (25 - Vertical) * window_size, -Reflector_half_x, Reflector.angle)) };
}
POINT Renderer::ReflectorPaint3(struct Power_Reflector Reflector, double Vertical) // 리플렉터 회전 전용 함수 - 3번 좌표
{
	return{ (long)(Pibot_x + PointRotationX(window_size * sqrt(Reflector.position * Reflector.position - Reflector.size * Reflector.size * 0.25) + Reflector_half_y + (25 + Vertical) * window_size, Reflector_half_x, Reflector.angle)),(long)(Pibot_y + PointRotationY(window_size * sqrt(Reflector.position * Reflector.position - Reflector.size * Reflector.size * 0.25) + Reflector_half_y + (25 + Vertical) * window_size, Reflector_half_x, Reflector.angle)) };
}

