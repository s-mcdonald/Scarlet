#pragma once

namespace Scarlet
{
    enum class ScarletRequest
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
}

