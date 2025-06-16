#pragma once

#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <cstring>

#include "../BaseResponse.hpp"

namespace Scarlet
{
    class HttpResponse : public BaseResponse
    {
        public:
            HttpResponse()
                : BaseResponse()
                , m_statusCode(200)
                , m_statusMessage("OK")
                , m_version("HTTP/1.1")
            {
            }

            void SetStatus(int code, const std::string& message)
            {
                m_statusCode = code;
                m_statusMessage = message;
            }

            void SetHeader(const std::string& key, const std::string& value)
            {
                if (key == "Content-Length")
                {
                    // throw error or exception as this should not be set manually
                }

                m_headers[key] = value;
            }

            void SetBody(const std::string& body)
            {
                m_body = body;
                m_headers["Content-Length"] = std::to_string(body.size());
            }

            [[nodiscard]] std::string GetRawResponse() const override
            {
                std::ostringstream response;

                response << m_version << " " << m_statusCode << " " << m_statusMessage << "\r\n";

                for (const auto& header : m_headers)
                {
                    response << header.first << ": " << header.second << "\r\n";
                }

                response << "\r\n";

                response << m_body;

                return response.str();
            }

            [[nodiscard]] const char* GetRawResponseAsCString() const override
            {
                return GetRawResponse().c_str();
            }

            [[nodiscard]] const size_t GetResponseBodySize() const override
            {
                return m_body.size();
            }

            void SetKeepAliveTimeout(const int timeout)
            {
                SetKeepAlive(timeout, -1);
            }

            void SetKeepAliveMax(const int max)
            {
                SetKeepAlive(-1, max);
            }

            void SetKeepAlive(const int timeout = -1, const int max = -1)
            {
                m_headers["Connection"] = "keep-alive";

                std::string keepAliveValue;

                if (timeout > 0)
                {
                    keepAliveValue += "timeout=" + std::to_string(timeout);
                }

                if (max > 0)
                {
                    if (!keepAliveValue.empty())
                    {
                        keepAliveValue += ", ";
                    }
                    keepAliveValue += "max=" + std::to_string(max);
                }

                if (!keepAliveValue.empty())
                {
                    m_headers["Keep-Alive"] = keepAliveValue;
                }
            }

        private:
            int m_statusCode;
            std::string m_statusMessage;
            std::string m_version;
            std::map<std::string, std::string> m_headers;
            std::string m_body;
    };
}
