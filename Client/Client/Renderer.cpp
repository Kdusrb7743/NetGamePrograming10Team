#include "stdafx.h"
#include "Renderer.h"


void Randerer::DisplayLoad()
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

void Randerer::DisplayColorApply()
{
	Reactor_RailImg.Load(TEXT("Img\\Reactor_Rail.png"));
	Reflector_ColorImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_LightImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_ColorChargeImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_LightChargeImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_ColorOffImg.Load(TEXT("Img\\Reflector_Color.png"));
	Reflector_LightOffImg.Load(TEXT("Img\\Reflector_Color.png"));

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

void Randerer::DoorIdle(HDC hdc)
{
	DoorImg.Draw(hdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
}

void Randerer::UIMenu(HDC hdc, bool Start, bool Module, bool Option, bool Quit)
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

void Randerer::UIButton(HDC hdc, int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30])
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

bool Randerer::UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse)
{
	return (Pibot_x + x * window_size < Mouse.x&& Mouse.x < Pibot_x + (x + sizex) * window_size && Pibot_y + y * window_size < Mouse.y&& Mouse.y < Pibot_y + (y + sizey) * window_size);
}


//void DisplayReflector(struct Power_Reflector* Reflector) // 리플렉터(반사판)들을 출력하는 함수
//{
//	if (Reflector->next != ReflectorHead)			// 리플렉터가 초기화 되지 않은 경우
//	{
//		POINT Reflector_Point[3] = {
//		ReflectorPaint1(Reflector->next, 0),
//		ReflectorPaint2(Reflector->next, 0),
//		ReflectorPaint3(Reflector->next, 0)
//		};
//		ReflectorImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Mask_Img, 0, 0);
//
//		if (Reflector->next->age < -100)
//		{
//			Reflector_ColorImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
//			Reflector_LightImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);
//		}
//		else if (Reflector->next->age > 0)
//		{
//			Reflector_ColorOffImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
//			Reflector_LightOffImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);
//		}
//		else
//		{
//			Reflector_ColorChargeImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Color_Mask_Img, 0, 0);
//			Reflector_LightChargeImg.PlgBlt(memdc, Reflector_Point, 0, 0, 375, 115, Reflector_Light_Mask_Img, 0, 0);
//		}
//
//		if (Reflector->next->effect > 0)
//		{
//			POINT Reflector_Effect_Point[3] = {
//			ReflectorPaint1(Reflector->next, 67.5),
//			ReflectorPaint2(Reflector->next, 67.5),
//			ReflectorPaint3(Reflector->next, 67.5)
//			};
//			Reflector_EffectImg.PlgBlt(memdc, Reflector_Effect_Point, 375 * (Reflector->next->effect % 100), (300 * (int)(Reflector->next->effect / 100.0)), 375, 300, Reflector_Effect_Mask_Img, 375 * (Reflector->next->effect % 100), 0);
//		}
//		DisplayReflector(Reflector->next);
//	}
//	else return;
//}