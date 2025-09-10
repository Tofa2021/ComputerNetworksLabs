#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;

int main() {
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1280);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (struct sockaddr*)&peer, sizeof(peer));

    int x, y;
    cout << "Enter coord x" << endl;
    cin >> x;
    cout << "Enter coord y" << endl;
    cin >> y;

    char coordString[255];
    sprintf_s(coordString, "%d %d", x, y);

    send(clientSocket, coordString, strlen(coordString) + 1, 0);

    char buffer[255];
    int bytesRecived;
    int quaterNumber;
    bytesRecived = recv(clientSocket, buffer, sizeof(buffer), 0);
    sscanf_s(buffer, "%d", &quaterNumber);
    
    cout << "Quarter number: " << quaterNumber;

    closesocket(clientSocket);
    WSACleanup();
}