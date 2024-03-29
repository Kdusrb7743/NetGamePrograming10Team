﻿// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define _USE_MATH_DEFINES				// 오류없이 파이값 불러오기
// Windows 헤더 파일
#include <windows.h>
// ImageLoad 헤더 파일
#include <atlImage.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <math.h>

#include "Define.h"
#include "GameFramework.h"
#include "Renderer.h"
#include "CNetworkManger.h"

using namespace std;

#define LOOPBACK "127.0.0.1" // "10.30.2.39"
#define NOW_IP LOOPBACK	

static Renderer gRender;

struct Power_Orb
{
	float x, y;
	short count;
	float size = 30;
};

struct Power_Reflector
{
	float angle, position, size;
};

extern struct Power_Orb Orb;

extern Power_Reflector Reflectors[3];

extern float PScore[3];


//플에이어 색상 정보
extern int Player1RGB[3];			// 색 정보
extern int Player2RGB[3];
extern int Player3RGB[3];

extern double window_half;
extern double window_size_x, window_size_y, Pibot_x, Pibot_y;

extern double Reactor_size, Rail_size, Orb_size;
extern double Controllroom_size_x, Reflector_size_x;
extern double Controllroom_size_y, Reflector_size_y;

extern int Reactor_window, Rail_window, Orb_window;
extern int Controllroom_window_x, Reflector_window_x;
extern int Controllroom_window_y, Reflector_window_y;

extern int Reactor_half, Rail_half, Orb_half;
extern int Controllroom_half_x, Reflector_half_x;
extern int Controllroom_half_y, Reflector_half_y;

//이미지 변수
extern CImage ReactorImg, Reactor_EffectImg, ReflectorImg, Reflector_EffectImg, OrbImg, PressureImg, CherenkovImg;
extern CImage Reactor_RailImg, Reflector_ColorImg, Reflector_LightImg, Reflector_ColorChargeImg, Reflector_LightChargeImg, Reflector_ColorOffImg, Reflector_LightOffImg;
extern CImage Reflector_Mask_Img, Reflector_Effect_Mask_Img, Reflector_Color_Mask_Img, Reflector_Light_Mask_Img;
extern CImage Button_PressureImg, Button_DialImg, Button_ValveImg, Button_OrbImg, Cherenkov_LeverImg, TempertureImg, DoorImg;
extern CImage Pressure_Mask_Img, Cherenkov_Mask_Img, Button_Valve_Mask_Img, Button_Dial_Mask_Img, Temperture_Mask_Img;
extern CImage Reflector_Module1_Img, Reflector_Module2_Img, Reflector_Module3_Img, Reflector_Module4_Img, Reflector_Module5_Img;
extern CImage Reflector_Module1_Mask_Img, Reflector_Module2_Mask_Img, Reflector_Module3_Mask_Img, Reflector_Module4_Mask_Img, Reflector_Module5_Mask_Img;
