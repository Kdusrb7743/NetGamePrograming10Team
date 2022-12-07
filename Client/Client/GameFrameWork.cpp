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
}

WGameFramework::~WGameFramework()
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
		else {
			gRender.DoorIdle(hdc);
			gRender.UIEndMessage(hdc);
		}

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

			//// 디버그 출력, UI-점수, 이펙트 출력
			//if (debug)	UIDebugInfo();
			gRender.UIScore(hdc);				//인자로 현재 자신이 몇번 플레이어인지에 따라 점수 출력부가 다름.
		}
	}
}

void WGameFramework::Update(short Left, short Right, const float frameTime)
{
	m_Net.recv_data(&m_SceneType, &m_SceneChange, &clientPID);				

	if (Left & 0x8000) {
		Reflectors[clientPID].angle += 0.01;
		cout << "왼쪽 이동" << endl;
	}
	if (Right & 0x8000) {
		Reflectors[clientPID].angle -= 0.01;
		cout << "오른쪽 이동" << endl;
	}

	switch(m_SceneType)
	{
	case Packet_Type::LOBBY:
		m_Net.Send(m_bReady);
		break;

	case Packet_Type::MAIN:
		m_Net.Send(Reflectors[clientPID].angle);
		break;

	case Packet_Type::END:
		m_Net.Send(m_bExitEndScene);
		break;

	case Packet_Type::NONE:
		m_Net.Send(false);
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
		//m_Modulebutton = gRender.UIButtonSelected(-700, 15, 750, 150, MousePoint);
		//m_Optionbutton = gRender.UIButtonSelected(-700, 165, 750, 150, MousePoint);
		//m_Quitbutton = gRender.UIButtonSelected(-700, 315, 750, 150, MousePoint);
		break;

	case WM_LBUTTONDOWN:
		if (m_Startbutton) {
			m_bReady = true;								//	Ready 변경점
			m_bExitEndScene = false;
		}
		if (m_SceneType == END)
		{
			m_bReady = false;
			m_bExitEndScene = true;
		}
		break;

	case WM_LBUTTONUP:
		break;

	default:
		return;
	}
}
