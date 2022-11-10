#include "stdafx.h"
#include "GameFramework.h"

WGameFramework::WGameFramework()
{
	// ��ư �ʱ�ȭ
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
	/*static int x = 10;
	x++;
	Rectangle(hdc, x + 10, 10, x + 100, 100);*/

	//hdc = BeginPaint(hWnd, &ps);
	//memdc = CreateCompatibleDC(hdc);
	//hBitmap = CreateCompatibleBitmap(hdc, (int)window_size_x, (int)window_size_y);

	//SelectObject(memdc, hBitmap);
	//SetStretchBltMode(memdc, COLORONCOLOR);
	//SetBkMode(memdc, TRANSPARENT);


	if (m_SceneType == LOBBY)				// ���� �κ� ���̸�
	{
		//���� ������ ��
		gRender.DoorIdle(hdc);
		gRender.UIMenu(hdc, m_Startbutton, m_Modulebutton, m_Optionbutton, m_Quitbutton);
	}
	else if (m_SceneType == END)			// ���� ���̸�
	{
		//���� ������ ��
		gRender.DoorIdle(hdc);
		//UIEndMessage();
	}
	else	//(m_SceneType == MAIN)				// MAIN ���̸�
	{
		//// ������� ���� �׸���
		//ReactorImg.Draw(hdc, Pibot_x - Controllroom_half_x, Pibot_y - Controllroom_half_y, Controllroom_window_x, Controllroom_window_y, 0, 0, Controllroom_size_x, Controllroom_size_y);
		//Reactor_RailImg.Draw(hdc, Pibot_x - 782 * window_half, Pibot_y - 782 * window_half, 782 * window_size, 782 * window_size, 0, 0, 782, 782);

		//// �ݻ��Ǳ׸���
		//DisplayReflector(ReflectorHead);

		////if (ReactorEffect < 12) Reactor_EffectImg.Draw(hdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size * (ReactorEffect % 6), Reactor_size * (int)(ReactorEffect / 6), Reactor_size, Reactor_size);
		////else Reactor_EffectImg.Draw(hdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, 5000, 1000, Reactor_size, Reactor_size);

		//DisplayOrb(OrbHead);						// �� �׸���

		//// ����� ���, UI-����, ����Ʈ ���
		//if (debug)	UIDebugInfo();

		//UIScore();
		//EffectPrint(EffectHead);

		//// �� �ִϸ��̼� ��			-- ���⼭ ������ �ִϸ��̼��� ��ü�� �׷��� �ִ� �տ� ������ ����ȴ�.
		//if (1) {					//�κ���� �� ���� ���
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
	//	��ŶŸ��(�������� ��Ŷ)�� �ް� �̸� �����ӿ�ũ Ŭ���� ��������� ����.
	m_Net.recv_fixed();
	if (m_SceneType != m_Net.getType())
	{
		m_SceneType = m_Net.getType();
		cout << "���� Scene Ÿ��: " << m_SceneType << endl;
	}
	
	


	//	���� Ű �Է��� ���� ���� Ŭ���̾�Ʈ ��ġ ���� ����Ѵ�.
	switch (m_SceneType)
	{
	case Packet_Type::LOBBY:
		m_Net.Send(m_bReady);
		break;

	case Packet_Type::MAIN:
		//	Ŭ���̾�Ʈ�� post position�� send()�Ѵ�. 
		//	������ ���� ���ε� post position�� recv()�Ѵ�.
	
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
		m_bReady = true;								//	Ready ������
		break;

	case WM_LBUTTONUP:
		break;

	default:
		return;
	}
}
