#pragma once
#include "CNetworkManger.h"

class WGameFramework
{
private:
	HWND					m_hWnd;
	CNetworkManger			m_Net;

private:
	Packet_Type				m_SceneType = NONE;
	bool					m_bReady;
	bool					m_bExitEndScene = false;
	int						clientPID;
public:				//	getter
	Packet_Type getType() { return m_SceneType; }

public:				//	setter
	void setType(Packet_Type pType) { m_SceneType = pType; }

public:
	bool					m_Startbutton;
	bool					m_Modulebutton;
	bool					m_Optionbutton;
	bool					m_Quitbutton;
	bool					m_SceneChange;
public:
	WGameFramework();
	~WGameFramework();

	virtual void Create(HWND hWnd);
	virtual void Render(HDC hdc);
	virtual void Update(short Left, short Right, const float frameTime = 0.17f);
	virtual void Mouse(UINT iMessage, WPARAM wParam, LPARAM lParam);

};