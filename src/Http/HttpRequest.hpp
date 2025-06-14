#pragma once

#include <map>
#include <sstream>
#include <string>

#include "ScarletRequestType.hpp"

namespace Scarlet
{
    class HttpRequest
    {
        public:
            explicit HttpRequest(const std::string &raw_request)
            {
                ParseRawRequest(raw_request);
            }

            std::string GetRawMethod() const
            {
                return m_rawMethod;
            }

            ScarletRequestType GetRequestType() const
            {
                return m_scarletMethod;
            }

            std::string GetRawPath() const
            {
                return m_path;
            }

            std::string GetRawVersion() const
            {
                return m_version;
            }

            std::map<std::string, std::string> GetRawHeaders() const
            {
                return m_headers;
            }

        private:
            void ParseRawRequest(const std::string &raw)
            {
                std::istringstream stream(raw);
                std::string line;

                if (std::getline(stream, line))
                {
                    std::istringstream request_line(line);

                    // what if we read mal formed data input

                    // if (request_line >> method >> path >> version) {
                    //
                    // } else {
                    //     // Malformed request line
                    // }

                    request_line >> m_rawMethod >> m_path >> m_version;

                    // simple parsing of req type to SRT (httpMethod -> ScarletRequestType)
                    if (m_rawMethod == "GET")
                    {
                        m_scarletMethod = ScarletRequestType::GET;
                    }
                    // we can add more later.
                }

                while (std::getline(stream, line) && line != "\r")
                {
                    auto colon = line.find(':');
                    if (colon == std::string::npos)
                    {
                        continue;
                    }

                    std::string key = line.substr(0, colon);
                    std::string value = line.substr(colon + 1);

                    auto trim = [](std::string& s)
                    {
                        s.erase(0, s.find_first_not_of(" \t\r\n"));
                        s.erase(s.find_last_not_of(" \t\r\n") + 1);
                    };

                    trim(key);
                    trim(value);

                    m_headers[key] = value;
                }

                if (m_headers.count("Content-Length"))
                {
                    int contentLength = std::stoi(m_headers["Content-Length"]);
                    m_body.resize(contentLength);
                    stream.read(&m_body[0], contentLength);
                }
            }

        private:
            ScarletRequestType m_scarletMethod{ScarletRequestType::PING};
            std::string m_rawMethod; // let's change this to enum class later
            std::string m_path;
            std::string m_version;
            std::map<std::string, std::string> m_headers;
            std::string m_body;
    };
}
