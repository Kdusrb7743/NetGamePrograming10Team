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

public:				//	getter
	Packet_Type getType() { return m_SceneType; }

public:				//	setter
	void setType(Packet_Type pType) { m_SceneType = pType; }

public:
	bool					m_Startbutton;
	bool					m_Modulebutton;
	bool					m_Optionbutton;
	bool					m_Quitbutton;

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

	//김연규 2주차 - Render() 추가부
	virtual void DisplayLoad();						// 이미지 로드
	virtual void DisplayColorApply();				// 이미지 색상 적용

	virtual void DoorIdle(HDC hdc);
	virtual void UIMenu(HDC hdc, bool Start, bool Module, bool Option, bool Quit);		// 로비씬 UI를 출력하는 함수
	virtual bool UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse);	// 버튼이 선택 됐는지
	virtual void UIButton(HDC hdc, int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30]);
};