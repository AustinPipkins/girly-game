#ifdef _WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <WS2tcpip.h>
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
    #define INVALID_SOCKET  (-1)
    #define SOCKET_ERROR    (-1)
    #define closesocket(s) close(s)
    typedef int SOCKET;
#endif

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <cstring>
#include <algorithm>


std::mutex clientsMutex;
std::vector<SOCKET> clients;

void initSockets() {
#ifdef _WIN32
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    if (WSAStartup(ver, &wsData) != 0) {
        std::cerr << "Can't start Winsock!" << std::endl;
        exit(1);
    }
#endif
}

void cleanupSockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}

// Broadcast message to all connected clients
void broadcastMessage(const std::string &message, SOCKET sender) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (SOCKET client : clients) {
        if (client != sender) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

void handleClient(SOCKET clientSocket, sockaddr_in clientInfo) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);
    std::cout << "Client connected: " << clientIP 
              << ":" << ntohs(clientInfo.sin_port) << std::endl;

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.push_back(clientSocket);
    }

    char buffer[4096];
    while (true) {
        memset(buffer, 0, 4096);
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived <= 0) {
            std::cout << "Client disconnected: " << clientIP << std::endl;
            break;
        }

        std::string msg = "[" + std::string(clientIP) + "] " + std::string(buffer);
        std::cout << msg << std::endl;
        broadcastMessage(msg, clientSocket);
    }

    closesocket(clientSocket);
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    }
}

int main() {
    initSockets();

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create socket!" << std::endl;
        cleanupSockets();
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "Can't bind to IP/port!" << std::endl;
        closesocket(listening);
        cleanupSockets();
        return -2;
    }

    listen(listening, SOMAXCONN);
    std::cout << "Server listening on port 54000..." << std::endl;

    while (true) {
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if (clientSocket == INVALID_SOCKET) continue;

        std::thread t(handleClient, clientSocket, client);
        t.detach();
    }

    closesocket(listening);
    cleanupSockets();
    return 0;
}
