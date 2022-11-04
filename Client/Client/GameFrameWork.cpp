#include "stdafx.h"
#include "GameFramework.h"


WGameFramework::WGameFramework()
{
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
}

void WGameFramework::Render(HDC hdc)
{
	static int x = 10;
	x++;
	Rectangle(hdc, x + 10, 10, x + 100, 100);
}

void WGameFramework::Update(const float frameTime)
{
	//	패킷타입(고정길이 패킷)을 받고 이를 프레임워크 클래스 멤버변수에 저장.
	m_Net.recv_fixed();
	m_SceneType = m_Net.getType();

	//	받은 키 입력을 토대로 다음 클라이언트 위치 값을 계산한다.
	int a = 0;
	
	//	클라이언트의 post position을 send()한다.
	 
	
	//	서버로 부터 승인된 post position을 recv()한다.


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
