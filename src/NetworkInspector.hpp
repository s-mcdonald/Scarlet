#pragma once

#include <string>

namespace Scarlet
{
    class NetworkInspector
    {
        public:
            NetworkInspector() = default;
            ~NetworkInspector() = default;

            static std::string Test();
    };
}
