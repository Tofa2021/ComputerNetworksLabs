#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <iostream>

using namespace std;

bool isDigit(char a) {
	char digits[] = "0123456789";
	for (int i = 0; i < 10; i++) {
		if (a == digits[i])
			return true;
	}

	return false;
}

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);

	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(1024);
	serverAddress.sin_addr.s_addr = 0;
	int serverAddressSize = sizeof(serverAddress);

	struct sockaddr_in clientAddress;
	int clientAddressSize = sizeof(clientAddress);

	bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

	char recivedBuffer[100];
	cout << "Server has been started" << endl;
	while (true) {
		int recivedBytesCount = recvfrom(serverSocket,
			recivedBuffer,
			sizeof(recivedBuffer) - 1,
			0,
			(struct sockaddr*)&clientAddress,
			&clientAddressSize);
		recivedBuffer[recivedBytesCount] = '\0';
		cout << "Recived string: " << recivedBuffer << endl;

		char sendBuffer[100];
		int j = 0;
		for (int i = 0; i < strlen(recivedBuffer); i++) {
			if (isDigit(recivedBuffer[i])) {
				sendBuffer[j] = recivedBuffer[i];
				j++;
			}
		}
		sendBuffer[j] = '\0';


		sendto(serverSocket, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr*) &clientAddress, clientAddressSize);
	}

	closesocket(serverSocket);
	WSACleanup();
}