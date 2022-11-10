#pragma once
#include "CNetworkManger.h"

class Randerer {
private:



public:
	//�迬�� 2���� - Render() �߰���
	virtual void DisplayLoad();						// �̹��� �ε�
	virtual void DisplayColorApply();				// �̹��� ���� ����

	virtual void DoorIdle(HDC hdc);
	virtual void UIMenu(HDC hdc, bool Start, bool Module, bool Option, bool Quit);		// �κ�� UI�� ����ϴ� �Լ�
	virtual bool UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse);	// ��ư�� ���� �ƴ���
	virtual void UIButton(HDC hdc, int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30]);
	//void DisplayReflector(struct Power_Reflector* Reflector)
};