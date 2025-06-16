#pragma once

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

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
            if (IsValid())
            {
                ::close(m_fd);
            }
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

        void Bind(uint16_t port) const
        {
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(port);

            if (bind(m_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
            //if (::bind(m_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
            {
                throw std::runtime_error("Failed to bind socket");
            }
        }

            // make this 10
        void Listen(int backlog = 3) const
        {
            if (::listen(m_fd, backlog) < 0)
            {
                throw std::runtime_error("Failed to listen on socket");
            }
        }

        Socket Accept(sockaddr_in* clientAddr = nullptr) const
        {
            socklen_t len = sizeof(sockaddr_in);
            sockaddr_in addr{};
            int new_fd = ::accept(m_fd, reinterpret_cast<sockaddr*>(&addr), &len);

            if (new_fd < 0)
            {
                throw std::runtime_error("Accept failed");
            }

            if (clientAddr)
            {
                *clientAddr = addr;
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

    private:
        int m_fd{-1};
    };
}
