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

	virtual void Reset();
	virtual void Clear();

	virtual void Create(HWND hWnd);
	virtual void Render(HDC hdc);
	virtual void Update(short Left, short Right, const float frameTime = 0.17f);
	virtual void PostUpdate(const float frameTime = 0.17f);

	virtual void KeyUpdate(UINT iMessage, WPARAM wParam, LPARAM lParam, short Left, short Right);

	virtual void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual void Mouse(UINT iMessage, WPARAM wParam, LPARAM lParam);

};