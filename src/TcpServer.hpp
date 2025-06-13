#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Logging/ServerLog.hpp"
#include <cstdlib>
#include <ctime>
#include <string>

#define LOCAL_PORT 8080
#define DEFAULT_PORT 80

// dev only - to check we are receiving a new req lets use a random number
inline int randomTwoDigit()
{
    return 10 + rand() % 90;
}

namespace Scarlet
{
    class TcpServer
    {
        public:
            TcpServer() :m_server_fd() {};
            ~TcpServer() = default;
            TcpServer(const TcpServer &) = delete;

            int Start()
            {
                Scarlet::ServerLog::info() << "ScarletServer attempt to create socket " << "...";
                m_server_fd = socket(AF_INET, SOCK_STREAM, 0);
                if (m_server_fd < 0) {
                    return 1;
                }

                // Bind to port
                Scarlet::ServerLog::info() << "ScarletServer attempt to bind to port " << LOCAL_PORT << "...";
                m_address.sin_family = AF_INET;
                m_address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
                m_address.sin_port = htons(LOCAL_PORT);
                if (bind(m_server_fd, (struct sockaddr*)&m_address, sizeof(m_address)) < 0)
                {
                    Scarlet::ServerLog::critical() << "ScarletServer bind failed " << "...\n";
                    perror("bind failed");
                    return 1;
                }

                return 0;
            }

            void StartListen()
            {
                Scarlet::ServerLog::info() << "ScarletServer Attempt to Listening on port " << LOCAL_PORT << "...";
                if (listen(m_server_fd, 3) < 0)
                {
                    Stop();
                }

                ssize_t bytesRead;

                while (true)
                {
                    // 4. Accept one connection
                    Scarlet::ServerLog::info() << "ScarletServer accepting connection " << "...";
                    m_new_socket = accept(m_server_fd, (struct sockaddr*)&m_address, (socklen_t*)&m_addrlen);

                    if (m_new_socket < 0) {
                        perror("accept failed");
                        continue;
                    }

                    // 5. Read request
                    Scarlet::ServerLog::info() << "ScarletServer reading request " << "...";
                    ssize_t bytesRead = read(m_new_socket, m_buffer, sizeof(m_buffer) - 1);
                    if (bytesRead > 0) {
                        m_buffer[bytesRead] = '\0';
                        Scarlet::ServerLog::debug() << "ScarletServer Received request:\n" << m_buffer;
                    }

                    // 6. Send basic HTTP response
                    Scarlet::ServerLog::info() << "ScarletServer sending response "  << "...";
                    int randomNumber = randomTwoDigit();
                    std::string body = "Hello world " + std::to_string(randomNumber);
                    std::string response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: " + std::to_string(body.size()) + "\r\n"
                        "Connection: close\r\n"
                        "\r\n" +
                        body;


                    send(m_new_socket, response.c_str(), response.size(), 0);
                    close(m_new_socket);
                    Scarlet::ServerLog::success() << "ScarletServer response sent OK "  << "...";
                }
            }

            void Stop() const
            {
                close(m_new_socket);
                close(m_server_fd);
            }

        private:
            int m_server_fd;
            int m_new_socket{};
            struct sockaddr_in m_address{};
            int m_addrlen = sizeof(m_address);
            char m_buffer[4096] = {0};
    };
}
