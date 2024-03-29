#include "Thread.h"

static HANDLE clientFlag[3];
static HANDLE processFlag;

int main(int argc, char* argv[])
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	processFlag = CreateEvent(NULL, TRUE, FALSE, NULL);
	for(int i = 0; i < 3; i++)
		clientFlag[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) return 1;

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) return 1;

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) return 1;

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	int clientNum = 0;
	HANDLE hThread[3];

	while (clientNum < 3) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) break;
		printf("���Ӽ���\n");
		hThread[clientNum] = CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL);
		if (hThread[clientNum] == NULL) { closesocket(client_sock); }

		clientNum++;
	}
	
	closesocket(listen_sock);
	WaitForMultipleObjects(3, hThread, true, INFINITE);
	for (int i = 0; i < 3; i++)
		CloseHandle(clientFlag[i]);
	WSACleanup();
	return 0;
}
