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

	gRender.DisplayLoad();
	gRender.DisplayColorApply();
}

void WGameFramework::Render(HDC hdc)
{

	static int Time = 0;

	if (m_SceneType == LOBBY)				// 현재 로비 씬이면
	{
		if (m_SceneChange == true)
		{
			m_SceneChange = false;
		}
		//닫힌 상태의 문
		gRender.DoorIdle(hdc);
		gRender.UIMenu(hdc, m_Startbutton, m_Modulebutton, m_Optionbutton, m_Quitbutton);
	}
	else if (m_SceneType == END)			// 엔드 씬이면
	{
		if (m_SceneChange == true)
		{
			gRender.DoorAnimation(hdc, Time--);
			if (Time == 0)
			{
				m_SceneChange = false;
			}
		}
		//닫힌 상태의 문
		gRender.DoorIdle(hdc);
		//UIEndMessage();
	}
	else	//(m_SceneType == MAIN)				// MAIN 씬이면
	{
		// 게임장과 레일 그리기
		if (m_SceneChange == true)
		{
			gRender.DoorAnimation(hdc, Time++);
			if (Time == 25)
			{
				m_SceneChange = false;
			}
		}

		else
		{
			// 게임장과 레일 그리기
			gRender.ReactorDraw(hdc);

			// 반사판그리기
			gRender.DisplayReflector(hdc);

			// 공 그리기
			gRender.DisplayOrb(hdc);

			//이펙트 부분
			//if (ReactorEffect < 12) Reactor_EffectImg.Draw(hdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size * (ReactorEffect % 6), Reactor_size * (int)(ReactorEffect / 6), Reactor_size, Reactor_size);
			//else Reactor_EffectImg.Draw(hdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, 5000, 1000, Reactor_size, Reactor_size);

			//// 디버그 출력, UI-점수, 이펙트 출력
			//if (debug)	UIDebugInfo();
			gRender.UIScore(hdc);				//인자로 현재 자신이 몇번 플레이어인지에 따라 점수 출력부가 다름.

			//EffectPrint(EffectHead);
		}
	}
}

void WGameFramework::Update(const float frameTime)
{
	//	패킷타입(고정길이 패킷)을 받고 이를 프레임워크 클래스 멤버변수에 저장.
	m_Net.recv_fixed();											// 2.고정 길이 패킷(씬타입) 전송
	if (m_SceneType != m_Net.getType())
	{
		m_SceneChange = true;
		m_SceneType = m_Net.getType();
		cout << "현재 Scene 타입: " << m_SceneType << endl;
	}
	
	


	//	받은 키 입력을 토대로 다음 클라이언트 위치 값을 계산한다.
	switch (m_SceneType)
	{
	case Packet_Type::LOBBY:
		m_Net.Send(m_bReady);
		//m_Net.Recv(char a);
		break;

	case Packet_Type::MAIN:

		//	클라이언트의 post position을 send()한다.	// 3. 각도값 전송

		m_Net.Send(Reflectors[0].angle);				// 일단 나의 각도 값만 보낸다.

		//	서버로 부터 승인된 post position을 recv()한다.

		m_Net.recv_variable();
	
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
			else if (wParam == VK_RIGHT)
			{
				//GetAsyncKeyState(VK_RIGHT);
				// 임시로 패널 움직이기
				Reflectors[0].angle -= 0.01;
				cout << "오른쪽으로 이동\n";
			}
			else if (wParam == VK_LEFT)
			{
				//GetAsyncKeyState(VK_RIGHT);
				// 임시로 패널 움직이기
				Reflectors[0].angle += 0.01;
				cout << "왼쪽으로 이동\n";
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
		m_Startbutton = gRender.UIButtonSelected(-700, -135, 750, 150, MousePoint);
		m_Modulebutton = gRender.UIButtonSelected(-700, 15, 750, 150, MousePoint);
		m_Optionbutton = gRender.UIButtonSelected(-700, 165, 750, 150, MousePoint);
		m_Quitbutton = gRender.UIButtonSelected(-700, 315, 750, 150, MousePoint);
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
