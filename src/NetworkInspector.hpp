#pragma once

#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Scarlet
{
    class NetworkInspector
    {
        public:
            NetworkInspector() = default;
            ~NetworkInspector() = default;

            std::string Test();
    };
}
