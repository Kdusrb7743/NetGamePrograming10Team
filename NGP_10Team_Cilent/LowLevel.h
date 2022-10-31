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
struct PositionData
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
DWORD WINAPI ProcessThread(LPVOID arg);					// 모든 계산을 수행
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


//---------------------------------------------------------------------------------- 2022.10.27 로우레벨 추가용
//	기본적으로 공과 패널은 연결리스트로 구현되어 있음
//  각도는 0 ~ 1
//	--인자 설명--
// Orb = 공
// Reflector = 패널(움직일 수 있는 반사판)
// 좌표 = x, y
// 각도 = Angle
// 공의 타입 = Type -- 삭제 예정
// dis = 거리
// 
// count = 패널모듈 추가 - 삭제
// Major = 메인 공 - 삭제

// -----------------------------------------------------------------서버
// Power_Orb.cpp
// 공 인자들(스피드, 각도, 좌표) 리셋 및 계산
void GeneralReset();												// 공, 버튼, 시간, 씬, 점수, 이펙트 타임 초기화
struct Power_Orb* OrbPosition(struct Power_Orb* Orb);				// 스피드에 따른 공 위치 계산
struct Power_Orb* OrbSpeed(struct Power_Orb* Orb);					// 공 스피드계산
void OrbCreate(struct Power_Orb* Orb, int Type,
	bool Major, float x, float y, float Angle);						// 메인 씬에서 불러줌. 공 생성시 객체 초기화
void OrbRemove(struct Power_Orb* NextOrb, struct Power_Orb* Orb);	// 공 하나 삭제
void OrbClear(struct Power_Orb* Orb);								// 공 전체 삭제 및 리셋용
struct Power_Orb* OrbApply(struct Power_Orb* Orb,
	int Type, bool Major, float x, float y, float Angle);			// 공 생성시 크기 초기화

// 공, 패널 충돌부분
void CollisionDetect(struct Power_Orb* Orb);						// 메인 공 충돌 했는지
struct Power_Orb* ReflectReflectorOrb(struct Power_Orb* Orb,
	struct Power_Reflector* Reflector);								// 공과 패널의 충돌 검사 후 공위치 조정
struct Power_Orb* ReflectOrb(struct Power_Orb* Orb, float Angle);	// 반사된 공의 각도 변경
void ReflectDetect(struct Power_Orb* Orb,
	struct Power_Reflector* Reflector);								// 반사판의 충돌 감지

//---------------------------------------------------------------------중요 !!클라 , 서버로 내부를 분리해야할 함수!!
void ReflectReflector(struct Power_Orb* Orb,
	struct Power_Reflector* Reflector);								// 반사판 충돌 검사 및 충돌하면 판에 대한 검수(점수, 튕김 애니메이션) 함수
//----------------------------------------------------------------------

// 반사판(패널) 인자들(스피드, 각도, 좌표) 리셋 및 계산
void ReflectorPosition(struct Power_Reflector* Reflector,
	short Left, short Right, short Up, short Down);					// 반사판의 위치를 매번 계산
void ReflectorCreate(struct Power_Reflector* Reflector, int Count);	// 반사판 생성시
void ReflectorRemove(struct Power_Reflector* NextReflector,
	struct Power_Reflector* Reflector);								// 반사판 하나 삭제
void ReflectClear(struct Power_Reflector* Reflector);				// 반사판 전체 삭제
struct Power_Reflector* ReflectorReset(struct Power_Reflector* Reflector);				// 반사판 리셋 -- 게임 시작시 리셋 후 다시 생성
struct Power_Reflector* ReflectorApply(struct Power_Reflector* Reflector, int Count);	// 반사판 적용


// Power_Math.cpp
float OrbScore(float Speed);										// 스피드에 따라 점수를 얻는다.
float AngleOverflow(float Angle);									// 각도 계산용 - 각도 360초과시 다시 0으로 돌린다.

float AnglePosition(float x, float y);								// 충돌용 - 절대 각도를 반환?
float Reflect(float Angle, float Reflector);						// 충돌용 - 반사된 각도를 반환(공 튕길 때 공 각도에 적용)
bool Distancecmp(float x, float y, float dis);						// 충돌용 - 거리가 레일 안쪽이면 트루
bool DistanceOvercmp(float x, float y, float dis);					// 충돌용 - 거리가 레일 밖으로 나갔을 때 트루
bool DistanceDetect(float x, float y, float Angle,
	float Distance, float Size);									// 충돌용 - 거리감지 계산용
bool AngleDetect(float x, float y, float Angle);					// 충돌시 - 패널각도 감지 계산용
bool ObtuseDetect(float Angle);										// 충돌용 - 둔각으로 충돌시
#pragma endregion


//------------------------------------------------------------------클라
// Power_Display.cpp는 클라에 전부 적용

struct Power_Effect								// 이펙트 용 구조체
{
	float x, y, score;
	int age;
	struct Power_Effect* next;
};

struct Power_Orb								// 공의 정보
{
	int type, effect, effect_count;				// 이펙트효과
	float x, y;									// 위치
	float afterx[25], aftery[25];				// 잔상 저장용 배열
	struct Power_Orb* next;
};

struct Power_Reflector							// 패널 정보
{
	float angle, position;
	int effect;
	struct Power_Reflector* next;
};



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

//	쓰레드 동기화용 Flag
HANDLE clientThreadFlag[3];
HANDLE processThreadFlag;
