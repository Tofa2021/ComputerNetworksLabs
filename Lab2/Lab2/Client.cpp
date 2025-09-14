#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <iostream>
#include <cstring>

using namespace std;

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);

	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in serverAddress;
	serverAddress.sin_port = htons(1024);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	int serverAddressSize = sizeof(serverAddress);

	cout << "Enter string" << endl;
	char sendBuffer[100];
	cin.getline(sendBuffer, 100, '\n');

	sendto(clientSocket, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr*) &serverAddress, serverAddressSize);

	char recivedBuffer[100];
	int recivedBytesCount = recvfrom(clientSocket,
									recivedBuffer,
									sizeof(recivedBuffer) - 1,
									0,
									(struct sockaddr*) &serverAddress,
									&serverAddressSize);
	recivedBuffer[recivedBytesCount] = '\0';
	cout << recivedBuffer << endl;

	closesocket(clientSocket);
	WSACleanup();
}