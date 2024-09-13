// client1.cpp (Windows)
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    float number = 5.75;  // Example float value

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Failed to initialize Winsock. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "Connection error" << std::endl;
        return 1;
    }

    // Send the float value to the server
    send(sock, (char*)&number, sizeof(number), 0);
    std::cout << "Float value sent to Server: " << number << std::endl;

    // Close the socket and clean up
    closesocket(sock);
    WSACleanup();

    return 0;
}

