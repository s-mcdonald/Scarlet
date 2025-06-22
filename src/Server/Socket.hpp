#pragma once

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

namespace Scarlet
{
    class Socket
    {
    public:
        Socket()
        {
            m_fd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (m_fd < 0) {
                throw std::runtime_error("Failed to create socket");
            }
        }

        explicit Socket(int fd) : m_fd(fd) {}

        ~Socket()
        {
            Close();
        }

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        // Movable
        Socket(Socket&& other) noexcept : m_fd(other.m_fd)
        {
            other.m_fd = -1;
        }

        Socket& operator=(Socket&& other) noexcept
        {
            if (this != &other)
            {
                if (IsValid())
                {
                    ::close(m_fd);
                }
                m_fd = other.m_fd;
                other.m_fd = -1;
            }
            return *this;
        }

        [[nodiscard]] bool Bind(uint16_t port) const
        {
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(port);

            if (bind(m_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
            {
                return false;
            }

            return true;
        }

        [[nodiscard]] bool Listen(int backlog = 10) const
        {
            if (::listen(m_fd, backlog) < 0)
            {
                return false;
            }

            return true;
        }

        [[nodiscard]] Socket Accept() const
        {
            socklen_t len = sizeof(sockaddr_in);
            sockaddr_in addr{};
            int new_fd = ::accept(m_fd, reinterpret_cast<sockaddr*>(&addr), &len);

            if (new_fd < 0)
            {
                throw std::exception();
            }

            return Socket(new_fd);
        }

        [[nodiscard]] int GetFD() const
        {
            return m_fd;
        }

        [[nodiscard]] bool IsValid() const
        {
            return m_fd >= 0;
        }

        void Close() const
        {
            if (IsValid())
            {
                ::close(m_fd);
            }
        }

        // Client Side interactions
        [[nodiscard]] bool Connect(const std::string& ip, uint16_t port) const
        {
            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0)
            {
                return false;
            }

            return ::connect(m_fd, (sockaddr*)&server_addr, sizeof(server_addr)) >= 0;
        }

        [[nodiscard]] ssize_t Send(const std::string& data) const
        {
            return ::send(m_fd, data.c_str(), data.size(), 0);
        }

        size_t Recv(char* buffer, size_t size) const
        {
            return ::recv(m_fd, buffer, size, 0);
        }


    private:
        int m_fd{-1};
    };
}
