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
	WSAStartup(wVersionRequested, &wsaData);

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(1280);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	listen(serverSocket, 5);
	cout << "Server has been started" << endl;
	while (true) {
		sockaddr_in clientAddress;
		int clientAddressSize = sizeof(clientAddress);

		SOCKET clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddress, &clientAddressSize);

		if (clientSocket != INVALID_SOCKET) {
			char ipString[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &clientAddress.sin_addr, ipString, sizeof(ipString));
			cout << "Client connected. IP: " << ipString << " "
				 << "Client Port: " << ntohs(clientAddress.sin_port) << endl;
			
			char recivedBuffer[255];
			while (int recivedBytesCount = recv(clientSocket, recivedBuffer, sizeof(recivedBuffer), 0) != 0) {
				recivedBuffer[recivedBytesCount] = '\0';

				int x, y, quaterNumber;
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
