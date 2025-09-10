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

	struct sockaddr_in localAddress;
	localAddress.sin_family = AF_INET;
	localAddress.sin_port = htons(1280);
	localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(serverSocket, (struct sockaddr*)&localAddress, sizeof(localAddress));

	listen(serverSocket, 5);
	cout << "Server was started" << endl;
	while (true) {
		sockaddr_in remoteAddress;
		int remoteAddressSize = sizeof(remoteAddress);

		SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&remoteAddress, &remoteAddressSize);

		if (clientSocket != INVALID_SOCKET) {
			char ipString[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &remoteAddress.sin_addr, ipString, sizeof(ipString));
			cout << "Client connected. IP: " << ipString << " "
				 << "Client Port: " << ntohs(remoteAddress.sin_port) << endl;
			
			char coordString[255];
			int bytesRecived;
			while (bytesRecived = recv(clientSocket, coordString, sizeof(coordString), 0) != 0) {
				coordString[bytesRecived] = '\0';

				int x, y, quaterNumber;
				sscanf_s(coordString, "%d %d", &x, &y);

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
