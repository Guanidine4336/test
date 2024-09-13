// udp_server.cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in servaddr, client1addr, client2addr;

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&client1addr, 0, sizeof(client1addr));
    memset(&client2addr, 0, sizeof(client2addr));

    // Filling server information
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof(client1addr);
    int n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&client1addr, &len);
    buffer[n] = '\0';
    std::cout << "Character received from Client1: " << buffer << std::endl;

    // Decrement the letter
    buffer[0] -= 1;

    // Send the decremented letter to Client2
    sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&client1addr, sizeof(client2addr));

    close(sockfd);
    return 0;
}
