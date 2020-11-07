#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 3333

int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct sockaddr_in addr;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}


	// Create a SOCKET for connecting to server
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	//addr.sin_addr.s_addr = inet_addr ("0.0.0.0"); 

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, (const sockaddr*)&addr, INET_ADDRSTRLEN);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	iResult = listen(ListenSocket, SOMAXCONN);  //If set to SOMAXCONN, the underlying service provider responsible for socket s will set the backlog to a maximum reasonable value
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("Server listening");

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);  // accept (SOCKET s, struct sockaddr *addr, *addrlen);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("Client accepted");

	// No longer need server socket
	closesocket(ListenSocket);

	// Receive until the peer shuts down the connection
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	while (iResult > 0)
	{
			printf("Bytes received: %d\n", iResult);

			recvbuf[iResult] = '\0';
			printf("Message: %s\n", recvbuf);
			strcat_s(recvbuf, " server confirmed.");

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult+20, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
			
			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	}
	if (iResult == 0)
			printf("Connection closing...\n");
	else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	system("pause");
	return 0;
}