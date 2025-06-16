#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <string>

#include "Logging/ServerLog.hpp"
#include "Http/HttpRequest.hpp"
#include "Http/HttpResponse.hpp"
#include "Socket.hpp"

#define LOCAL_PORT 8080
#define DEFAULT_PORT 80

namespace Scarlet
{
    class TcpServer
    {
        public:
            TcpServer() = default;
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

                // Bind to port
                Scarlet::ServerLog::info() << "ScarletServer attempt to bind to port " << LOCAL_PORT << "...";

                if (!serverSocket.Bind(8080))
                {
                    Scarlet::ServerLog::critical() << "ScarletServer bind failed " << "...\n";
                    return -1;
                }

                Scarlet::ServerLog::info() << "ScarletServer Attempt to Listening on port " << LOCAL_PORT << "...";
                if (!serverSocket.Listen())
                {
                    Scarlet::ServerLog::critical() << "ScarletServer Listen failed " << "...\n";
                    Stop();
                    return -1;
                }

                m_isRunning = true;

                return 0;
            }

            void StartListen()
            {
                //
                // While connection/server is running, lets accept inbound connections
                // only close connection for non-keep-alive req.
                //
                while (m_isRunning)
                {
                    sockaddr_in clientAddr{};
                    Scarlet::Socket m_new_socket = serverSocket.Accept(&clientAddr);

                    // still need to catch exception here..
                    Scarlet::ServerLog::info() << "ScarletServer accepting connection " << "...";

                    Scarlet::ServerLog::info() << "ScarletServer reading request " << "...";
                    ssize_t bytesRead = read(m_new_socket.GetFD(), m_buffer, sizeof(m_buffer) - 1);
                    if (bytesRead > 0)
                    {
                        m_buffer[bytesRead] = '\0';
                        Scarlet::ServerLog::debug() << "ScarletServer Received request:\n" << m_buffer;
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
                    responseObject.SetHeader("Content-Type", "text/plain");
                    responseObject.SetBody("Hello World!");

                    std::string raw = responseObject.GetRawResponse();
                    size_t bytesSent = send(m_new_socket.GetFD(), raw.c_str(), raw.size(), 0);

                    if (bytesSent < 0)
                    {
                        perror("send failed");
                    }
                    else
                    {
                        Scarlet::ServerLog::success() << "ScarletServer response sent OK "  << "...";
                    }

                    m_new_socket.Close();
                }
            }

            void Stop()
            {
                m_isRunning = false;
                serverSocket.Close();
            }

        private:
            bool m_isRunning = false;
            Scarlet::Socket serverSocket;
            char m_buffer[4096] = {0};
    };
}
