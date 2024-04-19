#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialisation Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    // Création de la socket serveur
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("Failed to create server socket\n");
        WSACleanup();
        return 1;
    }

    // Définition des informations de connexion du serveur
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Écoute sur toutes les interfaces réseau
    serverAddr.sin_port = htons(12345); // Port sur lequel le serveur écoute

    // Liaison de la socket à l'adresse et au port spécifiés
    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Failed to bind server socket\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Mise en écoute de la socket pour les connexions entrantes
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    OutputDebugString(L"Server listening on port 12345\n");

    // Accepter les connexions entrantes
    SOCKADDR_IN clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    OutputDebugString(L"Client connected\n");

    // Boucle pour recevoir les messages du client
    char buffer[1024];
    int bytesReceived;
    do {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            OutputDebugStringA("\nReceived message from client: ");
            OutputDebugStringA(buffer);
        }
    } while (bytesReceived > 0);

    // Fermeture des sockets et nettoyage de Winsock
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
