#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Logging/ServerLog.hpp"
#include "NetworkInspector.hpp"
#include "TcpServer.hpp"

#define LOCAL_PORT 8080
#define DEFAULT_PORT 80

// https://medium.com/@sakhawy/creating-an-http-server-from-scratch-ed41ef83314b
// https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7
int main()
{
    Scarlet::TcpServer tcpServer;
    Scarlet::NetworkInspector netInspector;

    Scarlet::ServerLog::info() << "ScarletServer initializing " << "...";
    if (tcpServer.Start() == 1)
    {
        perror("socket failed");
        Scarlet::ServerLog::critical() << "ScarletServer Failed to create socket connection ";
        return 1;
    }

    Scarlet::ServerLog::success() << "ScarletServer socket created " << "...";
    Scarlet::ServerLog::success() << "ScarletServer Binding to port " << LOCAL_PORT << "...";

    Scarlet::ServerLog::success() << "ScarletServer Listening on port " << LOCAL_PORT << "...";
    tcpServer.StartListen();

    Scarlet::ServerLog::success() << "ScarletServer shutting down "  << "...";
    tcpServer.Stop();
    return 0;
}