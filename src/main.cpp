#include <iostream>
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
    std::cout << "ScarletServer socket created " << "...\n";
    // end: minimal socket hookup for ip4

    // 2. Bind to port
    std::cout << "ScarletServer binding to port " << LOCAL_PORT << "...\n";
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    address.sin_port = htons(LOCAL_PORT);
    // 3. Listen
    std::cout << "ScarletServer listening on port " << LOCAL_PORT << "...\n";

    // 4. Accept one connection
    std::cout << "ScarletServer accepting connection " << "...\n";

    // 5. Read request
    std::cout << "ScarletServer reading request " << "...\n";

    // 6. Send basic HTTP response
    std::cout << "ScarletServer sending response \n";

    // 7. Close
    std::cout << "ScarletServer shutting down "  << "...\n";
    return 0;
}