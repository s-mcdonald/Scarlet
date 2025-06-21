#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "../BaseResponse.hpp"

namespace Scarlet
{
    class ScarletResponse : public BaseResponse
    {
        public:
            enum class Type
            {
                SIMPLE_STRING,
                ERROR,
                INTEGER,
                BULK_STRING,
                ARRAY
            };

            ScarletResponse() = default;

            static ScarletResponse createSimpleString(const std::string& value)
            {
                ScarletResponse res;
                res.m_type = Type::SIMPLE_STRING;
                res.m_single = value;
                return res;
            }

            static ScarletResponse createError(const std::string& errorMessage)
            {
                ScarletResponse res;
                res.m_type = Type::ERROR;
                res.m_single = errorMessage;
                return res;
            }

            static ScarletResponse createInteger(int64_t value)
            {
                ScarletResponse res;
                res.m_type = Type::INTEGER;
                res.m_integer = value;
                return res;
            }

            static ScarletResponse createBulkString(const std::string& value)
            {
                ScarletResponse res;
                res.m_type = Type::BULK_STRING;
                res.m_single = value;
                return res;
            }

            static ScarletResponse createNullBulkString()
            {
                ScarletResponse res;
                res.m_type = Type::BULK_STRING;
                res.m_isNull = true;
                return res;
            }

            static ScarletResponse createArray(const std::vector<ScarletResponse>& items)
            {
                ScarletResponse res;
                res.m_type = Type::ARRAY;
                res.m_array = items;
                return res;
            }

            [[nodiscard]] std::string GetRawResponse() const override
            {
                std::ostringstream out;

                switch (m_type)
                {
                    case Type::SIMPLE_STRING:
                        out << "+" << m_single << "\r\n";
                        break;
                    case Type::ERROR:
                        out << "-" << m_single << "\r\n";
                        break;
                    case Type::INTEGER:
                        out << ":" << m_integer << "\r\n";
                        break;
                    case Type::BULK_STRING:
                        if (m_isNull)
                        {
                            out << "$-1\r\n";
                        }
                        else
                        {
                            out << "$" << m_single.size() << "\r\n" << m_single << "\r\n";
                        }
                        break;
                    case Type::ARRAY:
                        out << "*" << m_array.size() << "\r\n";
                        for (const auto& item : m_array)
                        {
                            out << item.GetRawResponse();
                        }
                        break;
                }

                return out.str();
            }

            [[nodiscard]] const char* GetRawResponseAsCString() const override
            {
                m_buffer = GetRawResponse();
                return m_buffer.c_str();
            }

            [[nodiscard]] const size_t GetResponseBodySize() const override
            {
                return GetRawResponse().size();
            }

        private:
            mutable std::string m_buffer;
            Type m_type = Type::SIMPLE_STRING;
            std::string m_single;
            int64_t m_integer = 0;
            std::vector<ScarletResponse> m_array;
            bool m_isNull = false;
    };
}
