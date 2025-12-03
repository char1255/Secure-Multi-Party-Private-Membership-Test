#ifndef MRVF_EXC_HPP
#define MRVF_EXC_HPP

#include <stdexcept>
#include "core/mpmtcfg.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    class mrvf_exc : public std::runtime_error
    {
    public:
        enum class exc_type
        {
            IOFLOW_ERROR,                   // 打开文件失败
            RING_SIZE_MISMATCH,             // 文件RingSize与程序预设参数不匹配，
            FILE_CORRUPTION,                // 文件损坏 
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
            case exc_type::IOFLOW_ERROR:
                return "MRVF File I/O Error: " + info;

            case exc_type::FILE_CORRUPTION:
                return "MRVF Unexpected File Corruption: " + info;

            case exc_type::RING_SIZE_MISMATCH:
                return "MRVF Ring Size Mismatch Error: " + info;

            default:
                MPMT_WARN(false, "Undefined mrvf_exc::exc_type.");
                return "MRVF Unknown Exception: " + info;
            }
        }
    };
}
#endif // !MRVF_EXC_HPP
