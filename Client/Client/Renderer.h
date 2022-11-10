#pragma once
#include "CNetworkManger.h"

class Randerer {
private:



public:
	//김연규 2주차 - Render() 추가부
	virtual void DisplayLoad();						// 이미지 로드
	virtual void DisplayColorApply();				// 이미지 색상 적용

	virtual void DoorIdle(HDC hdc);
	virtual void UIMenu(HDC hdc, bool Start, bool Module, bool Option, bool Quit);		// 로비씬 UI를 출력하는 함수
	virtual bool UIButtonSelected(int x, int y, int sizex, int sizey, POINTS Mouse);	// 버튼이 선택 됐는지
	virtual void UIButton(HDC hdc, int x, int y, int R, int G, int B, int SR, int SG, int SB, bool Seleted, const TCHAR String[30]);
	//void DisplayReflector(struct Power_Reflector* Reflector)
};