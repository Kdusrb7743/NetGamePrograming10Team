#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h> 

#pragma comment(lib, "ws2_32")

enum PacketType		//Scene ����
{
	lobby,
	Main,
	end
};
// 0 -> 1 Ŭ���̾�Ʈ���� ������ �ϸ� ���� ����
// �κ�������� ������ϸ� �Ѱ��ָ� ���ΰ� ������

#pragma region [Struct]
struct PositionData
{
	float x = 0.f, y = 0.f;
};

struct clientData					// �������� �۾��� ��, �ʿ��� Ŭ���̾�Ʈ(User) ������
{
	PositionData m_clientPos;		// ������ ��ǥ
	PositionData m_clientNextPos;	// Ŭ���̾�Ʈ���� ���Ź��� ���� ��ǥ���� �ӽ÷� �����ϴ� ����
	float m_clientAngle;			// �г� �߽ɿ� �ش��ϴ� ������
	bool m_clientReady;
	unsigned short m_clientScore;
	char m_clientColor;				// RGB
};

struct ballData					// �������� �۾��� ��, �ʿ��� Ball ������
{
	PositionData m_ballPos;		// BallPosition
	float m_ballSpeed;			// 1Frame�� ���� �����̴� �Ÿ�
	float m_ballAngle;			// m_clientAngle �� N�� ���� ƨ��� ����
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
struct CS_LobbyPacket			// ������ ��
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
DWORD WINAPI ClientThread(LPVOID arg);					// Ŭ���̾�Ʈ���� ����� ����, 
DWORD WINAPI ProcessThread(LPVOID arg);					// ��� ����� ����
#pragma endregion

#pragma region [ServerFunction]
int SC_SendData(SOCKET client_sock);					// Ư�� Ŭ���̾�Ʈ�� ��Ŷ �����ϴ� �Լ�
int CS_RecvData(SOCKET client_sock);					//

#pragma region [LobbyScene]
bool ReadyCheck();										// �÷��̾���� �غ� �Ϸ��ߴ��� Ȯ���ϴ� �Լ�
void InitClient();										// Ŭ���̾�Ʈ ���ӽ� �ʱ�ȭ(��, ID)
#pragma endregion

#pragma region [GameScene]
bool BarCollision();									// �÷��̾� ���� �浹üũ�ϴ� �Լ� 
bool BallCollision();									// �÷��̾� �ٿ� �� �浹 ó���ϴ� �Լ� //���⼭ ���� ������ �ص� �ɰ� �����ϴ�. // �� �ӵ� ����
void CalBallPosition();									// �� ��ġ ��� �Լ�	// ������ �ӵ��� ���ؼ� ���� ��ġ ���
void InitBall();										// �� �ӵ� �� ���� �ʱ�ȭ, ���� �� ���� ����
bool EndCheck();										// ������ ����ƴ��� üũ�ϴ� �Լ�
#pragma endregion


//---------------------------------------------------------------------------------- 2022.10.27 �ο췹�� �߰���
//	�⺻������ ���� �г��� ���Ḯ��Ʈ�� �����Ǿ� ����
//  ������ 0 ~ 1
//	--���� ����--
// Orb = ��
// Reflector = �г�(������ �� �ִ� �ݻ���)
// ��ǥ = x, y
// ���� = Angle
// ���� Ÿ�� = Type -- ���� ����
// dis = �Ÿ�
// 
// count = �гθ�� �߰� - ����
// Major = ���� �� - ����

// -----------------------------------------------------------------����
// Power_Orb.cpp
// �� ���ڵ�(���ǵ�, ����, ��ǥ) ���� �� ���
void GeneralReset();												// ��, ��ư, �ð�, ��, ����, ����Ʈ Ÿ�� �ʱ�ȭ
struct Power_Orb* OrbPosition(struct Power_Orb* Orb);				// ���ǵ忡 ���� �� ��ġ ���
struct Power_Orb* OrbSpeed(struct Power_Orb* Orb);					// �� ���ǵ���
void OrbCreate(struct Power_Orb* Orb, int Type,
	bool Major, float x, float y, float Angle);						// ���� ������ �ҷ���. �� ������ ��ü �ʱ�ȭ
void OrbRemove(struct Power_Orb* NextOrb, struct Power_Orb* Orb);	// �� �ϳ� ����
void OrbClear(struct Power_Orb* Orb);								// �� ��ü ���� �� ���¿�
struct Power_Orb* OrbApply(struct Power_Orb* Orb,
	int Type, bool Major, float x, float y, float Angle);			// �� ������ ũ�� �ʱ�ȭ

// ��, �г� �浹�κ�
void CollisionDetect(struct Power_Orb* Orb);						// ���� �� �浹 �ߴ���
struct Power_Orb* ReflectReflectorOrb(struct Power_Orb* Orb,
	struct Power_Reflector* Reflector);								// ���� �г��� �浹 �˻� �� ����ġ ����
struct Power_Orb* ReflectOrb(struct Power_Orb* Orb, float Angle);	// �ݻ�� ���� ���� ����
void ReflectDetect(struct Power_Orb* Orb,
	struct Power_Reflector* Reflector);								// �ݻ����� �浹 ����

//---------------------------------------------------------------------�߿� !!Ŭ�� , ������ ���θ� �и��ؾ��� �Լ�!!
void ReflectReflector(struct Power_Orb* Orb,
	struct Power_Reflector* Reflector);								// �ݻ��� �浹 �˻� �� �浹�ϸ� �ǿ� ���� �˼�(����, ƨ�� �ִϸ��̼�) �Լ�
//----------------------------------------------------------------------

// �ݻ���(�г�) ���ڵ�(���ǵ�, ����, ��ǥ) ���� �� ���
void ReflectorPosition(struct Power_Reflector* Reflector,
	short Left, short Right, short Up, short Down);					// �ݻ����� ��ġ�� �Ź� ���
void ReflectorCreate(struct Power_Reflector* Reflector, int Count);	// �ݻ��� ������
void ReflectorRemove(struct Power_Reflector* NextReflector,
	struct Power_Reflector* Reflector);								// �ݻ��� �ϳ� ����
void ReflectClear(struct Power_Reflector* Reflector);				// �ݻ��� ��ü ����
struct Power_Reflector* ReflectorReset(struct Power_Reflector* Reflector);				// �ݻ��� ���� -- ���� ���۽� ���� �� �ٽ� ����
struct Power_Reflector* ReflectorApply(struct Power_Reflector* Reflector, int Count);	// �ݻ��� ����


// Power_Math.cpp
float OrbScore(float Speed);										// ���ǵ忡 ���� ������ ��´�.
float AngleOverflow(float Angle);									// ���� ���� - ���� 360�ʰ��� �ٽ� 0���� ������.

float AnglePosition(float x, float y);								// �浹�� - ���� ������ ��ȯ?
float Reflect(float Angle, float Reflector);						// �浹�� - �ݻ�� ������ ��ȯ(�� ƨ�� �� �� ������ ����)
bool Distancecmp(float x, float y, float dis);						// �浹�� - �Ÿ��� ���� �����̸� Ʈ��
bool DistanceOvercmp(float x, float y, float dis);					// �浹�� - �Ÿ��� ���� ������ ������ �� Ʈ��
bool DistanceDetect(float x, float y, float Angle,
	float Distance, float Size);									// �浹�� - �Ÿ����� ����
bool AngleDetect(float x, float y, float Angle);					// �浹�� - �гΰ��� ���� ����
bool ObtuseDetect(float Angle);										// �浹�� - �а����� �浹��
#pragma endregion


//------------------------------------------------------------------Ŭ��
// Power_Display.cpp�� Ŭ�� ���� ����

struct Power_Effect								// ����Ʈ �� ����ü
{
	float x, y, score;
	int age;
	struct Power_Effect* next;
};

struct Power_Orb								// ���� ����
{
	int type, effect, effect_count;				// ����Ʈȿ��
	float x, y;									// ��ġ
	float afterx[25], aftery[25];				// �ܻ� ����� �迭
	struct Power_Orb* next;
};

struct Power_Reflector							// �г� ����
{
	float angle, position;
	int effect;
	struct Power_Reflector* next;
};



#pragma region [ClientFunction]
void Recv_PacketType();									// �����κ��� ���� ��������(��)�� �޾� � ��Ŷ������ �޴��� �Ǵ��ϴ� �Լ�
int SC_RecvData();
void Render();


#pragma region [LobbyScene]
void SetID(char color);									// ������ ���� ID �ʱ�ȭ
void MouseInput(POINT point);							// ���콺 �Է� ��ġ�� ����üũ
int CS_Send_Ready(bool IsReady);						// ���� ������ ��Ŷ ����
#pragma endregion

#pragma region [GameScene]
char KeyInput();										// � Ű�� �Է� �ߴ��� ��ȯ�ϴ� �Լ�
void CalNextPosition();									// Ű�Է� �޾Ƽ� ���� ��ġ���� ����ϴ� �Լ�
int CS_Send_Position(PositionData clientPosition);		// ���� ��ġ�� ��Ŷ ����
														// Ű�� ���ȴٸ� ������ġ�� ���� or ������ ������ ���� ��ġ ����
#pragma endregion

#pragma endregion


// ��������
clientData Client[3];
ballData Ball;

//	������ ����ȭ�� Flag
HANDLE clientThreadFlag[3];
HANDLE processThreadFlag;
