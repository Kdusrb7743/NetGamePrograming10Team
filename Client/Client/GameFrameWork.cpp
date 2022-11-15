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

	static int Time = 0;

	if (m_SceneType == LOBBY)				// ���� �κ� ���̸�
	{
		if (m_SceneChange == true)
		{
			m_SceneChange = false;
		}
		//���� ������ ��
		gRender.DoorIdle(hdc);
		gRender.UIMenu(hdc, m_Startbutton, m_Modulebutton, m_Optionbutton, m_Quitbutton);
	}
	else if (m_SceneType == END)			// ���� ���̸�
	{
		if (m_SceneChange == true)
		{
			gRender.DoorAnimation(hdc, Time--);
			if (Time == 0)
			{
				m_SceneChange = false;
			}
		}
		//���� ������ ��
		gRender.DoorIdle(hdc);
		//UIEndMessage();
	}
	else	//(m_SceneType == MAIN)				// MAIN ���̸�
	{
		// ������� ���� �׸���
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
			// ������� ���� �׸���
			gRender.ReactorDraw(hdc);

			// �ݻ��Ǳ׸���
			gRender.DisplayReflector(hdc);

			// �� �׸���
			gRender.DisplayOrb(hdc);

			//����Ʈ �κ�
			//if (ReactorEffect < 12) Reactor_EffectImg.Draw(hdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, Reactor_size * (ReactorEffect % 6), Reactor_size * (int)(ReactorEffect / 6), Reactor_size, Reactor_size);
			//else Reactor_EffectImg.Draw(hdc, Pibot_x - Reactor_half, Pibot_y - Reactor_half, Reactor_window, Reactor_window, 5000, 1000, Reactor_size, Reactor_size);

			//// ����� ���, UI-����, ����Ʈ ���
			//if (debug)	UIDebugInfo();
			gRender.UIScore(hdc);				//���ڷ� ���� �ڽ��� ��� �÷��̾������� ���� ���� ��ºΰ� �ٸ�.

			//EffectPrint(EffectHead);
		}
	}
}

void WGameFramework::Update(const float frameTime)
{
	//	��ŶŸ��(�������� ��Ŷ)�� �ް� �̸� �����ӿ�ũ Ŭ���� ��������� ����.
	m_Net.recv_fixed();											// 2.���� ���� ��Ŷ(��Ÿ��) ����
	if (m_SceneType != m_Net.getType())
	{
		m_SceneChange = true;
		m_SceneType = m_Net.getType();
		cout << "���� Scene Ÿ��: " << m_SceneType << endl;
	}
	
	


	//	���� Ű �Է��� ���� ���� Ŭ���̾�Ʈ ��ġ ���� ����Ѵ�.
	switch (m_SceneType)
	{
	case Packet_Type::LOBBY:
		m_Net.Send(m_bReady);
		//m_Net.Recv(char a);
		break;

	case Packet_Type::MAIN:

		//	Ŭ���̾�Ʈ�� post position�� send()�Ѵ�.	// 3. ������ ����

		m_Net.Send(Reflectors[0].angle);				// �ϴ� ���� ���� ���� ������.

		//	������ ���� ���ε� post position�� recv()�Ѵ�.

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
				// �ӽ÷� �г� �����̱�
				Reflectors[0].angle -= 0.01;
				cout << "���������� �̵�\n";
			}
			else if (wParam == VK_LEFT)
			{
				//GetAsyncKeyState(VK_RIGHT);
				// �ӽ÷� �г� �����̱�
				Reflectors[0].angle += 0.01;
				cout << "�������� �̵�\n";
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
