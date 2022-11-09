#pragma once
#include "CNetworkManger.h"

class WGameFramework
{
private:
	HWND					m_hWnd;
	CNetworkManger			m_Net;

private:
	Packet_Type				m_SceneType = NONE;

	
public:				//	getter
	Packet_Type getType() { return m_SceneType; }

public:				//	setter
	void setType(Packet_Type pType) { m_SceneType = pType; }


public:
	WGameFramework();
	~WGameFramework();

	virtual void Reset();
	virtual void Clear();

	virtual void Create(HWND hWnd);
	virtual void Render(HDC hdc);
	virtual void Update(const float frameTime = 0.17f);
	virtual void PostUpdate(const float frameTime = 0.17f);

	virtual void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual void Mouse(UINT iMessage, WPARAM wParam, LPARAM lParam);

};

