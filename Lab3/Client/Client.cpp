#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>

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

    cout << "Enter string" << endl;
    string sendBuffer;
    cin >> sendBuffer;
    send(clientSocket, sendBuffer.c_str(), sizeof(sendBuffer), 0);

    char recivedBuffer[255];
    memset(recivedBuffer, '\0', sizeof(recivedBuffer));
    int recivedBytesCount = recv(clientSocket, recivedBuffer, sizeof(recivedBuffer), 0);
    cout << "Recived: " << recivedBuffer << endl;

    closesocket(clientSocket);
    WSACleanup();
}
