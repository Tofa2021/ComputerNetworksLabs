#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include<thread>
#include<vector>
#include<WinSock2.h>

using namespace std;

void clientHandler(SOCKET clientSocket) {
	char recivedBuffer[255];
	int recivedBytesCount;
	recivedBytesCount = recv(clientSocket, recivedBuffer, sizeof(recivedBuffer), 0);
	recivedBuffer[recivedBytesCount] = '\0';
	cout << "Recived: " << recivedBuffer << endl;

	for (int i = 0; i < strlen(recivedBuffer) - 1; i += 2) {
		swap(recivedBuffer[i], recivedBuffer[i + 1]);
	}

	cout << "Send: " << recivedBuffer << endl;
	send(clientSocket, recivedBuffer, strlen(recivedBuffer) + 1, 0);

	closesocket(clientSocket);
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

	int availableConnections = 10;
	cout << "Server has been started" << endl;
	vector<thread*> startedThreads(availableConnections);

	SOCKADDR_IN clientAdress;
	ZeroMemory(&clientAdress, sizeof(clientAdress));
	int sizeOfAddres = sizeof(clientAdress);
	for (size_t i = 0; i < availableConnections; i++)
	{
		SOCKET newConnection = accept(serverSocket, (sockaddr*)&clientAdress, &sizeOfAddres);
		if (newConnection == INVALID_SOCKET)
		{
			closesocket(newConnection);
			WSACleanup();
			cout << "Accept failure" << endl;
		}
		else
		{
			startedThreads[i] = new thread([&]() { clientHandler(newConnection); });
			cout << "New user connected! Thread ID:" << startedThreads[i]->get_id() << endl;
		}
	}
	for (size_t i = 0; i < startedThreads.size(); i++)
	{
		startedThreads[i]->join();
		delete startedThreads[i];
	}

	cout << "Server is offline!" << endl;
	closesocket(serverSocket);
	WSACleanup();
}
