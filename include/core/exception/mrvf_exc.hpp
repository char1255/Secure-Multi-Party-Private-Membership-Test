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
            FILE_OPEN_ERROR,                // 打开文件失败
            FILE_SEEK_ERROR,                // 读取文件指针偏移量时出错
            FILE_INVALID_BOF,               // 文件头错误
            FILE_INVALID_EOF,               // 文件尾错误
            FILE_RING_SIZE_MISMATCH         /** 文件RingSize与程序预设参数不匹配，
                                            例如文件值为16位环，但使用mrvf_handler<ring64>进行载入
                                            */
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
            case exc_type::FILE_OPEN_ERROR:
                return "MRVF File open error: " + info;

            case exc_type::FILE_SEEK_ERROR:
                return "MRVF File seek error: " + info;

            case exc_type::FILE_INVALID_BOF:
                return "MRVF Unexpected begin(header) of file: " + info;

            case exc_type::FILE_INVALID_EOF:
                return "MRVF Unexpected end(footer) of file: " + info;

            case exc_type::FILE_RING_SIZE_MISMATCH:
                return "MRVF Ring Size Mismatch Error: " + info;

            default:
                MPMT_WARN(false, "Undefined mrvf_exc::exc_type.");
                return "MRVF Unknown exception: " + info;
            }
        }
    };
}
#endif // !MRVF_EXC_HPP
