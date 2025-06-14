#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <utility>

namespace Scarlet
{
    class LogStream
    {
        public:
            LogStream(std::string  label, std::string  colorCode)
                : label(std::move(label)), color(std::move(colorCode)) {}

            ~LogStream()
            {
                flush();
            }

            template <typename T>
            LogStream& operator<<(const T& value)
            {
                stream << value;
                return *this;
            }

        private:
            void flush()
            {
                const std::string reset = "\033[0m";
                const std::string grey = "\033[90m";
                constexpr int labelWidth = 10;

                std::ostringstream labelStream;
                labelStream << "[" << label << "]";
                std::string paddedLabel = labelStream.str();
                if (paddedLabel.length() < labelWidth)
                    paddedLabel.append(labelWidth - paddedLabel.length(), ' ');

                std::cout << grey << timestamp() << reset << " "
                          << color << paddedLabel << reset << " "
                          << stream.str() << std::endl;
            }



            static std::string timestamp()
            {
                using namespace std::chrono;
                auto now = system_clock::now();
                auto itt = system_clock::to_time_t(now);
                auto tm = *std::localtime(&itt);

                std::ostringstream oss;
                oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
                return oss.str();
            }

            std::string label;
            std::string color;
            std::ostringstream stream;
    };

    struct ServerLog
    {
        static LogStream info()
        {
            return {"INFO", "\033[32m"};
        }
        static LogStream error()
        {
            return {"ERROR", "\033[31m"};
        }
        static LogStream warning()
        {
            return {"WARNING", "\033[33m"};
        }
        static LogStream debug()
        {
            return {"DEBUG", "\033[36m"};
        }
        static LogStream critical()
        {
            return {"CRITICAL", "\033[1;31m"};
        }
        static LogStream success()
        {
            return {"SUCCESS", "\033[1;32m"};

        }
    };
}
