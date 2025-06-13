#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "NetworkInspector.hpp"

#define LOCAL_PORT 8080
#define DEFAULT_PORT 80

// https://medium.com/@sakhawy/creating-an-http-server-from-scratch-ed41ef83314b
// https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7
int main()
{
    Scarlet::NetworkInspector netInspector;

    // 1. Create socket
    std::cout << "ScarletServer initializing " << "...\n";

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[4096] = {0};

    std::cout << "DEBUG: " << address.sin_port << "...\n";
    std::cout << "DEBUG: " << std::to_string(address.sin_addr.s_addr) << "...\n";

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, ipStr, INET_ADDRSTRLEN);
    std::cout << "IP Address: " << ipStr << "\n";

    // 1. Create socket
    std::cout << "ScarletServer attempt to create socket " << "...\n";
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }
    std::cout << "ScarletServer SUCCESS: socket created " << "...\n";
    // end: minimal socket hookup for ip4

    // 2. Bind to port
    std::cout << "ScarletServer binding to port " << LOCAL_PORT << "...\n";
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    address.sin_port = htons(LOCAL_PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }
    std::cout << "ScarletServer SUCCESS: binding to port " << LOCAL_PORT << "...\n";


    // 3. Listen
    std::cout << "ScarletServer listening on port " << LOCAL_PORT << "...\n";
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        return 1;
    }

    // 4. Accept one connection
    std::cout << "ScarletServer accepting connection " << "...\n";
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept failed");
        return 1;
    }

    // 5. Read request
    std::cout << "ScarletServer reading request " << "...\n";
    ssize_t bytesRead = read(new_socket, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Received request:\n" << buffer << "\n";
    }

    // 6. Send basic HTTP response
    std::cout << "ScarletServer sending response \n";
    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello world";
    send(new_socket, response, strlen(response), 0);
    std::cout << "Response sent\n";


    // 7. Close
    std::cout << "ScarletServer shutting down "  << "...\n";
    close(new_socket);
    close(server_fd);
    return 0;
}