#pragma once

#include <map>
#include <sstream>
#include <string>

namespace Scarlet
{
    class HttpResponse
    {
        public:
            HttpResponse()
                : m_statusCode(200),
                m_statusMessage("OK"),
                m_version("HTTP/1.1")
            {
            }

            void SetStatus(int code, const std::string& message)
            {
                m_statusCode = code;
                m_statusMessage = message;
            }

            void SetHeader(const std::string& key, const std::string& value)
            {
                m_headers[key] = value;
            }

            void SetBody(const std::string& body)
            {
                m_body = body;
                m_headers["Content-Length"] = std::to_string(body.size());
            }

            [[nodiscard]] std::string GetRawResponse() const
            {
                std::ostringstream response;

                response << m_version << " " << m_statusCode << " " << m_statusMessage << "\r\n";

                for (const auto& header : m_headers)
                {
                    response << header.first << ": " << header.second << "\r\n";
                }

                response << "\r\n"; // End of headers

                // Body
                response << m_body;

                return response.str();
            }

        private:
            int m_statusCode;
            std::string m_statusMessage;
            std::string m_version;
            std::map<std::string, std::string> m_headers;
            std::string m_body;
    };
}
