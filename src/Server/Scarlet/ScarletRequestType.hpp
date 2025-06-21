#pragma once
#include <string>

namespace Scarlet
{
    enum class ScarletRequestType
    {
        PING = 0,
        GET,
        SET,

        // We can support the other request later
        // btw, these are Redis requests, we might not want to replicvate all
        // HSET,
        // HGET,
        // LPUSH,
        // RPUSH,
        // LPOP,
        // RPOP,
        // SADD,
        // SREM,
        // ZADD,
        // ZREM,
        // DEL,
        // EXISTS,
        // INCR,
        // DECR,
    };

    inline std::string ScarletRequestTypeToString(ScarletRequestType type)
    {
        switch (type)
        {
            case ScarletRequestType::PING: return "PING";
            case ScarletRequestType::GET:  return "GET";
            case ScarletRequestType::SET:  return "SET";
                // Add more as needed
            default: return "UNKNOWN";
        }
    }
}

