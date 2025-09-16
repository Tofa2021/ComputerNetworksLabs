#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>

using namespace std;

int calculateQuaterNumber(int x, int y) {
	if (x == 0 || y == 0) {
		return 0;
	}

	if (x > 0) {
		if (y > 0) {
			return 1;
		}
		else {
			return 4;
		}
	}
	else {
		if (y > 0) {
			return 2;
		}
		else {
			return 3;
		}
	}
}

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) == WSAVERNOTSUPPORTED) {
		cout << "WSAStartup error" << endl;
		return -1;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		WSACleanup();
		cout << "Invalid socket" << endl;
		return -1;
	}

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(1280);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "Binding failed" << endl;
		return -1;
	}

	if (listen(serverSocket, SOMAXCONN)) {
		closesocket(serverSocket);
		WSACleanup();
		cout << "Listening failed" << endl;
		return -1;
	}

	cout << "Server has been started" << endl;
	while (true) {
		sockaddr_in clientAddress;
		int clientAddressSize = sizeof(clientAddress);

		SOCKET clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddress, &clientAddressSize);

		if (clientSocket != INVALID_SOCKET) {
			cout << "Client connected" << endl;

			char recivedBuffer[255];
			int recivedBytesCount;
			while ((recivedBytesCount = recv(clientSocket, recivedBuffer, sizeof(recivedBuffer), 0)) != 0) {
				if (recivedBytesCount < sizeof(recivedBuffer)) {
					recivedBuffer[recivedBytesCount] = '\0'; 
				}
				else {
					recivedBuffer[sizeof(recivedBytesCount) - 1] = '\n';
				}

				int x, y, quaterNumber;
				cout << "Recived string: " << recivedBuffer << endl;
				sscanf_s(recivedBuffer, "%d %d", &x, &y);

				quaterNumber = calculateQuaterNumber(x, y);

				char result[100];
				sprintf_s(result, "%d", quaterNumber);
				send(clientSocket, result, strlen(result) + 1, 0);
			}
		}

		closesocket(clientSocket);
	}

	closesocket(serverSocket);
	WSACleanup();
}
