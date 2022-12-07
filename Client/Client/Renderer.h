#pragma once
#include "CNetworkManger.h"


class Renderer {
private:
	float M_TU = M_PI * 2;


public:
	//�迬�� 2���� - Render() �κ�
	void DisplayLoad();						// �̹��� �ε�
	void DisplayColorApply();				// �̹��� ���� ����

	void DoorIdle(HDC hdc);														// �κ�� �� Draw												
	void UIMenu(HDC hdc, bool Start, bool Module, bool Option, bool Quit);		// �κ�� UI�� ����ϴ� �Լ�
	bool UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse);	// ��ư�� ���� �ƴ���
	void UIButton(HDC hdc, int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30]);
	void DoorAnimation(HDC hdc, int Time);												// �� �ִϸ��̼�
	void UIScore(HDC hdc);
	void UIEndMessage(HDC hdc);
	
	// MAIN ����� Draw
	void ReactorDraw(HDC hdc);

	// ��, �г� ��ü Draw
	void DisplayReflector(HDC hdc);
	void DisplayOrb(HDC hdc);

public:
	// ������ �̹��� ȸ���� ����Լ� 
	float PointRotationX(float x, float y, float angle);					// �̹��� ȸ�� ����� ���� X��ǥ ���
	float PointRotationY(float x, float y, float angle);					// �̹��� ȸ�� ����� ���� y��ǥ ���
	POINT ReflectorPaint1(struct Power_Reflector Reflector, double Vertical);
	POINT ReflectorPaint2(struct Power_Reflector Reflector, double Vertical);
	POINT ReflectorPaint3(struct Power_Reflector Reflector, double Vertical);
};