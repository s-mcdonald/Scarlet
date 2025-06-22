#include <iostream>
#include "TcpClient.hpp"

int main()
{
    using namespace Scarlet;

    try {
        TcpClient client;

        if (!client.Connect("127.0.0.1", 8080)) {
            std::cerr << "Failed to connect to server\n";
            return 1;
        }

        std::string request = "GET / HTTP/1.1\r\n"
                              "Host: localhost\r\n"
                              "Connection: close\r\n\r\n";

        client.SendRequest(request);

        std::string response = client.ReceiveAll();
        std::cout << response;

        client.Close();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
