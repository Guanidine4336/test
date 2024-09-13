// server.cpp (Windows)
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET server_fd, client1_fd, client2_fd;
    struct sockaddr_in server, client1, client2;
    char buffer[1024] = {0};
    int c, valread;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Failed to initialize Winsock. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Listen for connections
    listen(server_fd, 3);

    std::cout << "Waiting for incoming connections..." << std::endl;

    // Accept client1 connection
    c = sizeof(struct sockaddr_in);
    client1_fd = accept(server_fd, (struct sockaddr *)&client1, &c);
    if (client1_fd == INVALID_SOCKET) {
        std::cerr << "Accept failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Receive character from client1
    valread = recv(client1_fd, buffer, sizeof(buffer), 0);
    std::cout << "Character received from Client1: " << buffer << std::endl;

    // Decrement the letter
    buffer[0] -= 1;

    // Accept client2 connection
    client2_fd = accept(server_fd, (struct sockaddr *)&client2, &c);
    if (client2_fd == INVALID_SOCKET) {
        std::cerr << "Accept failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Send the decremented letter to client2
    send(client2_fd, buffer, strlen(buffer), 0);

    // Close the sockets
    closesocket(client1_fd);
    closesocket(client2_fd);
    closesocket(server_fd);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
