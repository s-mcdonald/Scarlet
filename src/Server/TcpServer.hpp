#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string>

#include "Logging/ServerLog.hpp"
#include "Http/HttpRequest.hpp"
#include "Http/HttpResponse.hpp"
#include "Scarlet/ScarletResponse.hpp"
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
                if (!serverSocket.Bind(LOCAL_PORT))
                {
                    Scarlet::ServerLog::critical() << "ScarletServer binding socket to port failed " << "...\n";
                    return -1;
                }

                if (!serverSocket.Listen())
                {
                    Scarlet::ServerLog::critical() << "ScarletServer Attempt to Listening on port failed " << LOCAL_PORT << "...";
                    Stop();
                    return -1;
                }

                m_isRunning = true;

                return 0;
            }

            void StartListen()
            {
                Scarlet::ServerLog::success() << "ScarletServer initialized and running " << "...";

                while (m_isRunning)
                {
                    Scarlet::ServerLog::info() << "ScarletServer waiting for request " << "...";

                    Socket clientSocket;

                    try {
                        clientSocket = serverSocket.Accept();
                    }
                    catch (const std::exception& e) {
                        Scarlet::ServerLog::error() << "ScarletServer failed accepting connection " << "...";
                        continue;
                    }

                    Scarlet::ServerLog::success() << "ScarletServer request received " << "...";

                    ssize_t bytesRead = read(clientSocket.GetFD(), m_buffer, sizeof(m_buffer) - 1);
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


                    // decide depending on inbound connection if we need a HttpResponse or a scarlet Response
                    // only require http response for non ScarletClients or directApi requests.
                    ScarletResponse responseObject = ScarletResponse::createBulkString("foo\r\nbar");

                    // HttpResponse responseObject;
                    // responseObject.SetHeader("Content-Type", "text/plain");
                    // responseObject.SetHeader("Scarlet-Data-Type", "integer");
                    // responseObject.SetBody("123");

                    std::string raw = responseObject.GetRawResponse();
                    size_t bytesSent = send(clientSocket.GetFD(), raw.c_str(), raw.size(), 0);

                    if (bytesSent < 0)
                    {
                        perror("send failed");
                    }
                    else
                    {
                        Scarlet::ServerLog::success() << "ScarletServer response sent OK "  << "...";
                    }

                    clientSocket.Close();
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
