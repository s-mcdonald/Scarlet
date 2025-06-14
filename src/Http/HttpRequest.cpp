#pragma once

#include <string>
#include <map>

namespace Scarlet
{
    class HttpRequest
    {
        public:
            explicit HttpRequest(const std::string &raw_request)
            {
                ParseRawRequest(raw_request);
            }

        private:
            void ParseRawRequest(const std::string &raw)
            {
                // ..
            }

        private:
            std::string m_method; // let's change this to enum class later
            std::string m_path;
            std::string m_version;
            std::map<std::string, std::string> m_headers;
            std::string m_body;
    };
}
