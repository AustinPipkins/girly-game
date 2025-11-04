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
#include <cstring>

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

void receiveMessages(SOCKET sock) {
    char buffer[4096];
    while (true) {
        memset(buffer, 0, 4096);
        int bytesReceived = recv(sock, buffer, 4096, 0);
        if (bytesReceived <= 0) {
            std::cout << "Disconnected from server.\n";
            break;
        }
        std::cout << "\n" << std::string(buffer, bytesReceived) << std::endl;
        std::cout << "> " << std::flush;
    }
}

int main() {
    initSockets();

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Can't create socket!" << std::endl;
        cleanupSockets();
        return -1;
    }

    std::string serverIP;
    std::cout << "Enter server IP: ";
    std::cin >> serverIP;
    std::cin.ignore();

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        std::cerr << "Can't connect to server!" << std::endl;
        closesocket(sock);
        cleanupSockets();
        return -2;
    }

    std::thread receiver(receiveMessages, sock);
    receiver.detach();

    std::string userInput;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (userInput == "quit" || userInput == "exit") break;

        send(sock, userInput.c_str(), userInput.size(), 0);
    }

    closesocket(sock);
    cleanupSockets();
    return 0;
}
