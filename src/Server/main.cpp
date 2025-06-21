#include "TcpServer.hpp"
#include "Logging/ServerLog.hpp"

// https://medium.com/@sakhawy/creating-an-http-server-from-scratch-ed41ef83314b
// https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7
int main()
{
    Scarlet::TcpServer tcpServer;

    Scarlet::ServerLog::info() << "ScarletServer initializing " << "...";
    if (tcpServer.Start() == -1)
    {
        Scarlet::ServerLog::critical() << "ScarletServer Failed to create socket connection ";
        return 1;
    }

    tcpServer.StartListen();

    Scarlet::ServerLog::success() << "ScarletServer shutting down "  << "...";
    tcpServer.Stop();
    return 0;
}