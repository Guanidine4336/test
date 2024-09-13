// server.cpp (Windows)
#include <iostream>
#include <winsock2.h>
#include <cmath>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET server_fd, client1_fd, client2_fd;
    struct sockaddr_in server, client1, client2;
    float received_value = 0.0, modified_value = 0.0;
    int c;

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

    std::cout << "Waiting for Client1..." << std::endl;

    // Accept client1 connection
    c = sizeof(struct sockaddr_in);
    client1_fd = accept(server_fd, (struct sockaddr *)&client1, &c);
    if (client1_fd == INVALID_SOCKET) {
        std::cerr << "Accept failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Receive float value from client1
    recv(client1_fd, (char*)&received_value, sizeof(received_value), 0);
    std::cout << "Float value received from Client1: " << received_value << std::endl;

    // Modify the float value by raising it to the power of 1.5
    modified_value = pow(received_value, 1.5);
    std::cout << "Modified value to be sent to Client2: " << modified_value << std::endl;

    // Accept client2 connection
    std::cout << "Waiting for Client2..." << std::endl;
    client2_fd = accept(server_fd, (struct sockaddr *)&client2, &c);
    if (client2_fd == INVALID_SOCKET) {
        std::cerr << "Accept failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Send the modified float value to client2
    send(client2_fd, (char*)&modified_value, sizeof(modified_value), 0);

    // Close the sockets
    closesocket(client1_fd);
    closesocket(client2_fd);
    closesocket(server_fd);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}

