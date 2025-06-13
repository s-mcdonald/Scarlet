#pragma once

// will eventually move code and refactor into the Server class..
namespace Scarlet
{
    class TcpServer
    {
        public:
            TcpServer() = default;
            ~TcpServer() = default;
            TcpServer(const TcpServer &) = delete;

        static int Start()
        {
            return 0;
        }
    };
}


