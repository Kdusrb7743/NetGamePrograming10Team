#include "stdafx.h"
#include "GameFramework.h"


WGameFramework::WGameFramework()
{
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
}

void WGameFramework::Render(HDC hdc)
{
	static int x = 10;
	x++;
	Rectangle(hdc, x + 10, 10, x + 100, 100);
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

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
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
