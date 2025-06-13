#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Logging/ServerLog.hpp"
#include "NetworkInspector.hpp"

#define LOCAL_PORT 8080
#define DEFAULT_PORT 80

// https://medium.com/@sakhawy/creating-an-http-server-from-scratch-ed41ef83314b
// https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7
int main()
{
    Scarlet::NetworkInspector netInspector;

    // 1. Create socket
    Scarlet::ServerLog::info() << "ScarletServer initializing " << "...";

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[4096] = {0};

    // std::cout << "DEBUG: " << address.sin_port << "...\n";
    // std::cout << "DEBUG: " << std::to_string(address.sin_addr.s_addr) << "...\n";

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, ipStr, INET_ADDRSTRLEN);
    // std::cout << "IP Address: " << ipStr << "\n";

    // 1. Create socket
    Scarlet::ServerLog::info() << "ScarletServer attempt to create socket " << "...";
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        Scarlet::ServerLog::critical() << "ScarletServer Failed to create socket connection ";
        return 1;
    }
    Scarlet::ServerLog::success() << "ScarletServer socket created " << "...";
    // end: minimal socket hookup for ip4

    // 2. Bind to port
    Scarlet::ServerLog::info() << "ScarletServer attempt to bind to port " << LOCAL_PORT << "...";
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    address.sin_port = htons(LOCAL_PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        Scarlet::ServerLog::critical() << "ScarletServer bind failed " << "...\n";
        perror("bind failed");
        return 1;
    }
    Scarlet::ServerLog::success() << "ScarletServer Binding to port " << LOCAL_PORT << "...";


    // 3. Listen
    Scarlet::ServerLog::info() << "ScarletServer Attempt to Listening on port " << LOCAL_PORT << "...";
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        Scarlet::ServerLog::critical() << "ScarletServer Failed to Listening on port " << LOCAL_PORT << "...";
        return 1;
    }
    Scarlet::ServerLog::success() << "ScarletServer Listening on port " << LOCAL_PORT << "...";

    // 4. Accept one connection
    Scarlet::ServerLog::info() << "ScarletServer accepting connection " << "...";
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept failed");
        return 1;
    }

    // 5. Read request
    Scarlet::ServerLog::info() << "ScarletServer reading request " << "...";
    ssize_t bytesRead = read(new_socket, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        Scarlet::ServerLog::debug() << "ScarletServer Received request:\n" << buffer;
    }

    // 6. Send basic HTTP response
    Scarlet::ServerLog::info() << "ScarletServer sending response "  << "...";
    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello world";
    send(new_socket, response, strlen(response), 0);
    Scarlet::ServerLog::success() << "ScarletServer response sent OK "  << "...";


    // 7. Close
    Scarlet::ServerLog::success() << "ScarletServer shutting down "  << "...";
    close(new_socket);
    close(server_fd);
    return 0;
}