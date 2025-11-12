#ifndef RING_FILE_HPP
#define RING_FILE_HPP

#include <string>
#include "core/ring/ring.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    template<typename T>
    class rvector_file
    {
    public:
        /** @brief 断言限制模板类型 */
        static_assert(
            std::is_same_v<RT, ring1> ||
            std::is_same_v<RT, ring8> ||
            std::is_same_v<RT, ring16> ||
            std::is_same_v<RT, ring32> ||
            std::is_same_v<RT, ring64>,
            "RT must be ring1, ring8, ring16, ring32, or ring64."
            );

        rvector_file(const std::string& file_path){}

        ~rvector_file();

        static const uint32_t RVECTOR_FILE_HEAD = 0x4D504D54; // 文件头 "MPMT"

        // 文件尾
        const uint8_t m_ring_size_;   // 环大小
        T* const m_buffer;            // 数据
        const size_t m_size;          // 数据字节数
    };
}

#endif // !RING_FILE_HPP