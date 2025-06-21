#pragma once

#include <string>

namespace Scarlet
{
    class BaseResponse
    {
        public:
            virtual ~BaseResponse() = default;

            [[nodiscard]] virtual std::string GetRawResponse() const = 0;

            [[nodiscard]] virtual const char* GetRawResponseAsCString() const = 0;

            [[nodiscard]] virtual const size_t GetResponseBodySize() const = 0;
    };
}
