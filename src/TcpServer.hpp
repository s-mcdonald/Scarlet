#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <string>

#include "Logging/ServerLog.hpp"
#include "Http/HttpRequest.hpp"
#include "Http/HttpResponse.hpp"

#define LOCAL_PORT 8080
#define DEFAULT_PORT 80

namespace Scarlet
{
    class TcpServer
    {
        public:
            TcpServer() :m_server_fd() {};
            ~TcpServer() = default;
            TcpServer(const TcpServer &) = delete;

            //
            // Attempt to start the server by creating a socket, obtaining
            // a file descriptor, and binding it to an available port.
            // If binding fails, retry in a loop. (WIP)
            //
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

                m_isRunning = true;

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

                //
                // While connection/server is running, lets accept inbound connections
                // only close connection for non-keep-alive req.
                //
                while (m_isRunning)
                {
                    // 4.
                    Scarlet::ServerLog::info() << "ScarletServer accepting connection " << "...";
                    m_new_socket = accept(m_server_fd, (struct sockaddr*)&m_address, (socklen_t*)&m_addrlen);

                    if (m_new_socket < 0) {
                        perror("accept failed");
                        continue;
                    }

                    Scarlet::ServerLog::info() << "ScarletServer reading request " << "...";
                    ssize_t bytesRead = read(m_new_socket, m_buffer, sizeof(m_buffer) - 1);
                    if (bytesRead > 0) {
                        m_buffer[bytesRead] = '\0';
                       // Scarlet::ServerLog::debug() << "ScarletServer Received request:\n" << m_buffer;
                        Scarlet::HttpRequest request{std::string(m_buffer)};
                        Scarlet::ServerLog::debug() << "(!) New Request Received: ";
                        Scarlet::ServerLog::debug() << " - Parsed Scarlet Req Type: " << ScarletRequestTypeToString(request.GetRequestType());
                        Scarlet::ServerLog::debug() << " - Parsed Http Req: " << request.GetRawMethod();
                        Scarlet::ServerLog::debug() << " - Parsed Path: " << request.GetRawPath();
                        Scarlet::ServerLog::debug() << " - Parsed Version: " << request.GetRawVersion();
                        Scarlet::ServerLog::debug() << " - Parsed Headers Count: " << request.GetRawHeaders().size();
                    }




                    Scarlet::ServerLog::info() << "ScarletServer sending response "  << "...";
                    HttpResponse responseObject;
                    responseObject.SetHeader("Content-Type", "application/json"); // text/plain
                    responseObject.SetBodyDev();

                    send(m_new_socket, responseObject.GetRawResponseAsCString(), responseObject.GetResponseBodySize(), 0);
                    close(m_new_socket); // dont do this for keep alive conenctions
                    Scarlet::ServerLog::success() << "ScarletServer response sent OK "  << "...";
                }
            }

            void Stop()
            {
                close(m_new_socket);
                close(m_server_fd);
                m_isRunning = false;
            }

        private:
            int m_server_fd;
            int m_new_socket{};
            struct sockaddr_in m_address{};
            int m_addrlen = sizeof(m_address);
            char m_buffer[4096] = {0};
            bool m_isRunning = false;
    };
}
