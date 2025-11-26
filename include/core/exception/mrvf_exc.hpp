#ifndef MRVF_EXC_HPP
#define MRVF_EXC_HPP

#include <stdexcept>
#include <mpmtcfg.hpp>

/** @namespace 项目命名空间 */
namespace mpmt
{
    class mrvf_exc : public std::runtime_error
    {
    public:
        enum class exc_type
        {
            OPEN_ERROR,                 // 打开文件失败
            INVALID_BOF,                // 文件头错误
            INVALID_EOF,                // 文件尾错误
            IFAILED,                    // 文件读入错误
            OFAILED,                    // 文件写入错误
            RING_SIZE_MISMATCH,         // 文件RingSize与程序预设参数不匹配，
            CRC64_CHECK_FAILED,         // CRC64校验失败
        };

        explicit mrvf_exc
        (
            const exc_type& type,
            const std::string& info
        ) :
            m_type(type),
            std::runtime_error(build_message(type, info))
        {}
    private:
        exc_type m_type;

        static std::string build_message(exc_type type, const std::string& info)
        {
            switch (type)
            {
            case exc_type::OPEN_ERROR:
                return "MRVF File open error: " + info;

            case exc_type::INVALID_BOF:
                return "MRVF Invalid begin(header) of file: " + info;

            case exc_type::INVALID_EOF:
                return "MRVF Invalid end(footer) of file: " + info;

            case exc_type::RING_SIZE_MISMATCH:
                return "MRVF Ring Size Mismatch Error: " + info;

            case exc_type::CRC64_CHECK_FAILED:
                return "MRVF CRC64 Check Failed: " + info;

            default:
                MPMT_WARN(false, "Undefined mrvf_exc::exc_type.");
                return "MRVF Unknown exception: " + info;
            }
        }
    };
}
#endif // !MRVF_EXC_HPP
