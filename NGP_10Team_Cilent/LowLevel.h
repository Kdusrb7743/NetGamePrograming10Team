#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h> 

#pragma comment(lib, "ws2_32")

enum PacketType		//Scene 정보
{
	lobby,
	Main,
	end
};
// 0 -> 1 클라이언트에서 수신을 하면 게임 시작
// 로비씬에서는 헤더파일만 넘겨주면 끝인거 같은데

#pragma region [Struct]
struct PositionData	//double로 해야될수도...
{
	float x = 0.f, y = 0.f;
};
struct clientData					// 서버에서 작업할 때, 필요한 클라이언트(User) 데이터
{
	PositionData m_clientPos;		// 중점의 좌표
	PositionData m_clientNextPos;	// 클라이언트에게 수신받은 다음 좌표값을 임시로 저장하는 변수
	float m_clientAngle;			// 패널 중심에 해당하는 각도값
	bool m_clientReady;
	unsigned short m_clientScore;
	char m_clientColor;				// RGB
};
struct ballData					// 서버에서 작업할 때, 필요한 Ball 데이터
{
	PositionData m_ballPos;		// BallPosition
	float m_ballSpeed;			// 1Frame당 공이 움직이는 거리
	float m_ballAngle;			// m_clientAngle ± N에 들어가면 튕기게 설정
	short m_ballCount;
};
#pragma endregion

#pragma region [Packet]
#pragma region [Server to Client]
struct SC_headerFile		// SceneData
{
	PacketType m_type;
};

struct SC_LobbyPacket
{
	char m_clientColor[3];
};

struct SC_MainPacket
{
	PositionData m_ballPos;
	PositionData m_clientPos[3];
	unsigned short m_clientScore;
};

struct SC_EndPacket
{
	unsigned short m_clientScore[3];
};
#pragma endregion

#pragma region [Client to Server]
struct CS_LobbyPacket			// 수신할 떄
{
	bool m_clientReady = false;
};

struct CS_MainPacket
{
	PositionData m_clientPos;
};
#pragma endregion
#pragma endregion

#pragma region [Thread]
DWORD WINAPI ClientThread(LPVOID arg);					// 클라이언트와의 통신을 수행, 
DWORD WINAPI CalculateThread(LPVOID arg);				// 모든 계산을 수행
#pragma endregion

#pragma region [ServerFunction]
int SC_SendData(SOCKET client_sock);					// 특정 클라이언트에 패킷 전송하는 함수
int CS_RecvData(SOCKET client_sock);					//

#pragma region [LobbyScene]
bool ReadyCheck();										// 플레이어들이 준비를 완료했는지 확인하는 함수
void InitClient();										// 클라이언트 접속시 초기화(색, ID)
#pragma endregion

#pragma region [GameScene]
bool BarCollision();									// 플레이어 끼리 충돌체크하는 함수 
bool BallCollision();									// 플레이어 바와 공 충돌 처리하는 함수 //여기서 점수 계산까지 해도 될거 같습니다. // 공 속도 증가
void CalBallPosition();									// 공 위치 계산 함수	// 각도와 속도를 통해서 다음 위치 계산
void InitBall();										// 공 속도 및 각도 초기화, 남은 공 갯수 감소
bool EndCheck();										// 게임이 종료됐는지 체크하는 함수
#pragma endregion

#pragma endregion



#pragma region [ClientFunction]
void Recv_PacketType();									// 서버로부터 받은 고정길이(씬)을 받아 어떤 패킷구조를 받는지 판단하는 함수
int SC_RecvData();
void Render();


#pragma region [LobbyScene]
void SetID(char color);									// 서버가 받은 ID 초기화
void MouseInput(POINT point);							// 마우스 입력 위치로 레디체크
int CS_Send_Ready(bool IsReady);						// 레디를 했으면 패킷 전송
#pragma endregion

#pragma region [GameScene]
char KeyInput();										// 어떤 키를 입력 했는지 반환하는 함수
void CalNextPosition();									// 키입력 받아서 다음 위치값을 계산하는 함수
int CS_Send_Position(PositionData clientPosition);		// 다음 위치값 패킷 전송
														// 키가 눌렸다면 다음위치를 전송 or 눌리지 않으면 현재 위치 전송
#pragma endregion

#pragma endregion


// 전역변수
clientData Client[3];
ballData Ball;