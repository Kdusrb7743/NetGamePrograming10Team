#include "stdafx.h"
#include "GameFramework.h"

WGameFramework::WGameFramework()
{
	// 버튼 초기화
	m_Startbutton = false;
	m_Modulebutton = false;
	m_Optionbutton = false;
	m_Quitbutton = false;

	m_bReady = false;
	Clear();
}

WGameFramework::~WGameFramework()
{
}

void WGameFramework::Reset()
{
}

void WGameFramework::Clear()
{
}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;
	m_Net.init();

	DisplayLoad();
	DisplayColorApply();
}

void WGameFramework::Render(HDC hdc)
{
	/*static int x = 10;
	x++;
	Rectangle(hdc, x + 10, 10, x + 100, 100);*/

	//hdc = BeginPaint(hWnd, &ps);
	//memdc = CreateCompatibleDC(hdc);
	//hBitmap = CreateCompatibleBitmap(hdc, (int)window_size_x, (int)window_size_y);

	//SelectObject(memdc, hBitmap);
	//SetStretchBltMode(memdc, COLORONCOLOR);
	//SetBkMode(memdc, TRANSPARENT);


	if (m_SceneType == LOBBY)				// 현재 로비 씬이면
	{
		//닫힌 상태의 문
		DoorIdle(hdc);
		UIMenu(hdc, m_Startbutton, m_Modulebutton, m_Optionbutton, m_Quitbutton);
	}
	else if (m_SceneType == END)			// 엔드 씬이면
	{
		//닫힌 상태의 문
		DoorIdle(hdc);
		//UIEndMessage();
	}
	else	//(m_SceneType == MAIN)				// MAIN 씬이면
	{
		//// 게임장과 레일 그리기
		//ReactorImg.Draw(hdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
		//Reactor_RailImg.Draw(hdc, Pibot_x - 782 * window_half, Pibot_y - 782 * window_half, 782 * window_size, 782 * window_size, 0, 0, 782, 782);

		//// 반사판그리기
		//DisplayReflector(ReflectorHead);

		////if (ReactorEffect < 12) Reactor_EffectImg.Draw(hdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size * (ReactorEffect % 6), Reactor_size * (int)(ReactorEffect / 6), Reactor_size, Reactor_size);
		////else Reactor_EffectImg.Draw(hdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, 5000, 1000, Reactor_size, Reactor_size);

		//DisplayOrb(OrbHead);						// 공 그리기

		//// 디버그 출력, UI-점수, 이펙트 출력
		//if (debug)	UIDebugInfo();

		//UIScore();
		//EffectPrint(EffectHead);

		//// 문 애니메이션 씬			-- 여기서 열리는 애니메이션이 객체가 그려져 있는 앞에 덧씌워 진행된다.
		//if (1) {					//로비씬에 막 왔을 경우
		//	DoorAnimation();
		//}
	}
	//BitBlt(hdc, 0, 0, (int)window_size_x, (int)window_size_y, memdc, 0, 0, SRCCOPY);
	//DeleteObject(hBitmap);
	//DeleteDC(memdc);

	//-----------------------------------------------------------------------------------------------------------------------------//

	//EndPaint(hWnd, &ps);


}

void WGameFramework::Update(const float frameTime)
{
	//	패킷타입(고정길이 패킷)을 받고 이를 프레임워크 클래스 멤버변수에 저장.
	m_Net.recv_fixed();
	if (m_SceneType != m_Net.getType())
	{
		m_SceneType = m_Net.getType();
		cout << "현재 Scene 타입: " << m_SceneType << endl;
	}
	
	


	//	받은 키 입력을 토대로 다음 클라이언트 위치 값을 계산한다.
	switch (m_SceneType)
	{
	case Packet_Type::LOBBY:
		m_Net.Send(m_bReady);
		break;

	case Packet_Type::MAIN:
		//	클라이언트의 post position을 send()한다. 
		//	서버로 부터 승인된 post position을 recv()한다.
	
		break;

	case Packet_Type::END:
		break;

	case Packet_Type::NONE:
		break;
	}
	

}

void WGameFramework::PostUpdate(const float frameTime)
{
}

void WGameFramework::KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_KEYDOWN:
		{
			if (wParam == VK_Q)
			{
				SendMessage(m_hWnd, WM_DESTROY, 0, 0);
				return;
			}
		}
		break;
		

		case WM_KEYUP:
		{

		}
		break;


	}

}

void WGameFramework::Mouse(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	DWORD x, y;

	POINTS MousePoint = MAKEPOINTS(lParam);

	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		//POINTS MousePoint = MAKEPOINTS(lParam);
		m_Startbutton = (UIButtonSelected(-700, -135, 750, 150, MousePoint));
		m_Modulebutton = UIButtonSelected(-700, 15, 750, 150, MousePoint);
		m_Optionbutton = UIButtonSelected(-700, 165, 750, 150, MousePoint);
		m_Quitbutton = UIButtonSelected(-700, 315, 750, 150, MousePoint);
		break;

	case WM_LBUTTONDOWN:
		if (LOBBY) {

		}
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		cout << "x: " << x << ", y: " << y << endl;
		m_bReady = true;								//	Ready 변경점
		break;

	case WM_LBUTTONUP:
		break;

	default:
		return;
	}
}

void WGameFramework::DisplayLoad()
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

void WGameFramework::DisplayColorApply()
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

void WGameFramework::DoorIdle(HDC hdc)
{
	DoorImg.Draw(hdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
}

void WGameFramework::UIMenu(HDC hdc, bool Start, bool Module, bool Option, bool Quit)
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

void WGameFramework::UIButton(HDC hdc, int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30])
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

bool WGameFramework::UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse)
{
	return (Pibot_x + x * window_size < Mouse.x&& Mouse.x < Pibot_x + (x + sizex) * window_size && Pibot_y + y * window_size < Mouse.y&& Mouse.y < Pibot_y + (y + sizey) * window_size);
}
