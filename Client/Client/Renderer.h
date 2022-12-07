#pragma once
#include "CNetworkManger.h"


class Renderer {
private:
	float M_TU = M_PI * 2;


public:
	//김연규 2주차 - Render() 부분
	void DisplayLoad();						// 이미지 로드
	void DisplayColorApply();				// 이미지 색상 적용

	void DoorIdle(HDC hdc);														// 로비씬 문 Draw												
	void UIMenu(HDC hdc, bool Start, bool Module, bool Option, bool Quit);		// 로비씬 UI를 출력하는 함수
	bool UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse);	// 버튼이 선택 됐는지
	void UIButton(HDC hdc, int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30]);
	void DoorAnimation(HDC hdc, int Time);												// 문 애니메이션
	void UIScore(HDC hdc);
	void UIEndMessage(HDC hdc);
	
	// MAIN 경기장 Draw
	void ReactorDraw(HDC hdc);

	// 공, 패널 객체 Draw
	void DisplayReflector(HDC hdc);
	void DisplayOrb(HDC hdc);

public:
	// 렌더용 이미지 회전점 계산함수 
	float PointRotationX(float x, float y, float angle);					// 이미지 회전 출력을 위한 X좌표 계산
	float PointRotationY(float x, float y, float angle);					// 이미지 회전 출력을 위한 y좌표 계산
	POINT ReflectorPaint1(struct Power_Reflector Reflector, double Vertical);
	POINT ReflectorPaint2(struct Power_Reflector Reflector, double Vertical);
	POINT ReflectorPaint3(struct Power_Reflector Reflector, double Vertical);
};