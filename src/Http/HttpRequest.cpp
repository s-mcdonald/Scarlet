#pragma once

#include <string>
#include <map>
#include <sstream>
#include <algorithm>

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
                // Create a stream from the input
                std::istringstream stream(raw);
                std::string line;

                // parse the entire stream
                if (std::getline(stream, line))
                {
                    std::istringstream request_line(line);
                    request_line >> m_method >> m_path >> m_version;
                }

                // once parsed set the local member values
                // now parse all the headers
                // get the content body

                while (std::getline(stream, line) && line != "\r")
                {
                    auto colon = line.find(":");
                    if (colon != std::string::npos)
                    {

                    }
                }
            }

        private:
            std::string m_method; // let's change this to enum class later
            std::string m_path;
            std::string m_version;
            std::map<std::string, std::string> m_headers;
            std::string m_body;
    };
}
