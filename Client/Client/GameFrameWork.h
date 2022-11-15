#pragma once
class CNetworkManger;

class WGameFramework
{
private:
	HWND					m_hWnd;
	CNetworkManger*			m_Net;

private:
	Packet_Type				m_SceneType = NONE;
	bool					m_bReady;

public:				//	getter
	Packet_Type getType() { return m_SceneType; }
	bool		getReady() { return m_bReady; }

public:				//	setter
	void setNet(CNetworkManger* _n) { m_Net = _n; }
	void setType(Packet_Type pType) { m_SceneType = pType; }
	void setReady(bool _b) { m_bReady = _b; }

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
	virtual void Update(const float frameTime = 0.17f);
	virtual void PostUpdate(const float frameTime = 0.17f);

	virtual void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual void Mouse(UINT iMessage, WPARAM wParam, LPARAM lParam);

};