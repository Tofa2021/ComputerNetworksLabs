#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) == WSAVERNOTSUPPORTED) {
        cout << "WSAStartup error" << endl;
        return -1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        cout << "Invalid socket" << endl;
        return -1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(1280);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        closesocket(clientSocket);
        cout << "Client socket error" << endl;
        return -1;
    }

    int x, y;
    cout << "Enter coord x" << endl;
    cin >> x;
    cout << "Enter coord y" << endl;
    cin >> y;

    char sendBuffer[255];
    sprintf_s(sendBuffer, "%d %d", x, y);

    send(clientSocket, sendBuffer, strlen(sendBuffer) + 1, 0);

    char recivedBuffer[255];
    int recivedBytesCount = recv(clientSocket, recivedBuffer, sizeof(recivedBuffer), 0);
    int quaterNumber;
    sscanf_s(recivedBuffer, "%d", &quaterNumber);

    cout << "Quarter number: " << quaterNumber;

    closesocket(clientSocket);
    WSACleanup();
}