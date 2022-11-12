#pragma once

#define MAIN_TIMER			1	
#define MAIN_TIEMR_FRAME	17 // 60FPS

//#define FRAME_WIDTH		1280
//#define FRAME_HEIGHT		720

#define window_size			0.6f		//0.96
#define FRAME_WIDTH			2000.f * window_size
#define FRAME_HEIGHT		1125.f * window_size

// KeyBoard
enum VK_KEY
{
	VK_0 = 48,
	VK_1 = 49,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,
	VK_A = 0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z
};

enum Packet_Type
{
	NONE,
	LOBBY,
	MAIN,
	END,
};

struct PositionData
{
	float x = 0.f, y = 0.f;
};