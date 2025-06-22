#pragma once

#include "Socket.hpp"
#include <string>
#include <iostream>

namespace Scarlet
{
    class TcpClient
    {
        public:
            TcpClient() = default;

            bool Connect(const std::string& ip, uint16_t port)
            {
                return m_socket.Connect(ip, port);
            }

            bool SendRequest(const std::string& request)
            {
                return m_socket.Send(request) >= 0;
            }

            std::string ReceiveAll()
            {
                std::string response;
                char buffer[4096];
                ssize_t bytesRead;

                while ((bytesRead = m_socket.Recv(buffer, sizeof(buffer) - 1)) > 0)
                {
                    buffer[bytesRead] = '\0';
                    response += buffer;
                }

                return response;
            }

            void Close()
            {
                m_socket.Close();
            }

        private:
            Socket m_socket;
    };
}
