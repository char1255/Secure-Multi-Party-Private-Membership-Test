#ifndef MRVF_EXC_HPP
#define MRVF_EXC_HPP

#include <stdexcept>

/** @namespace 项目命名空间 */
namespace mpmt
{
    class mrvf_exc : public std::runtime_error
    {
    public:
        enum class error_type
        {
            FILE_OPEN_ERROR,
            FILE_SEEK_ERROR,
            FILE_UNEXPECTED_BOF,
            FILE_UNEXPECTED_EOF,
            SIZE
        };

        explicit mrvf_exc
        (
            const error_type& type,
            const std::string& info
        ) : 
            m_type(type),
            std::runtime_error(build_message(type, info)) 
        {}
    private:
        error_type m_type;

        static std::string build_message(error_type type, const std::string& info)
        {
            switch (type)
            {
                case error_type::FILE_OPEN_ERROR        :   return "MRVF File open error: " + info;
                case error_type::FILE_SEEK_ERROR        :   return "MRVF File seek error: " + info;
                case error_type::FILE_UNEXPECTED_BOF    :   return "MRVF Unexpected begin(header) of file: " + info;
                case error_type::FILE_UNEXPECTED_EOF    :   return "MRVF Unexpected end(footer) of file: " + info;
                default:                                    return "MRVF Unknown exception: " + info;
            }
        }
    };
}
#endif // !MRVF_EXC_HPP
