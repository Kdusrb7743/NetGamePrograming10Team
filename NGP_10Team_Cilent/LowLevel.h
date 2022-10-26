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
struct PositionData	//double�� �ؾߵɼ���...
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
DWORD WINAPI CalculateThread(LPVOID arg);				// ��� ����� ����
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

#pragma endregion



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