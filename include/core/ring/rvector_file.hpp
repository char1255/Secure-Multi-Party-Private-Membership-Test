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
            std::is_same_v<T, ring1> ||
            std::is_same_v<T, ring8> ||
            std::is_same_v<T, ring16> ||
            std::is_same_v<T, ring32> ||
            std::is_same_v<T, ring64>,
            "T must be ring1, ring8, ring16, ring32, or ring64."
            );

        /** @brief 唯一允许的构造函数 */
        explicit rvector_file(const std::string& load_path)
        {
            // 检查八字节头是否为MPMT
            // 读入八字节长度， 暂时限定为2^35最大，保留2^64
            // 顺次读入数据
            // 检查文件尾
            // 你无需关注其他情况，我自己会处理，你先写一个示例
        }

        /** @brief 构造函数 */
        void save(const std::string& save_path);

        std::unique_ptr<T[]> release_buffer()
        {
            return std::move(this->m_buffer);
        }

        /** @brief 析构函数 */
        ~rvector_file();


    private:
        /** @brief constant */
        static inline constexpr uint64_t SOF_LEN = 8;                   // 文件头标识长度（单位：字节）
        static inline constexpr uint64_t COF_LEN = 8;                   // 文件尾标识长度（单位：字节）
        static inline constexpr uint64_t CRC_LEN = 8;                   // 循环冗余校验码长度（单位：字节）
        static inline constexpr uint64_t SOF_64 = 0x464F535F544D504D;   // 文件头-标识 "MPMT_SOF"
        static inline constexpr uint64_t EOF_64 = 0x464F455F544D504D;   // 文件尾-标识 "MPMT_EOF"

        /** @brief 禁用拷贝与移动操作 */
        const uint8_t m_ring_size_;     // 文件头-环大小（2, 2^8, 2^16, 2^32, 2^64）
        const uint64_t m_size;          // 文件头-数据量, 支持m_size个T类型数据, 共计 m_size*sizeof(T) bytes
        std::unique_ptr<T[]> m_buffer;  // 数据载荷段

        /** @brief 禁用拷贝与移动操作 */
        rvector_file() = delete;                                    // 禁止默认构造
        rvector_file(const rvector_file&) = delete;                 // 禁止拷贝构造
        rvector_file(rvector_file&&) = delete;                      // 禁止移动构造
        rvector_file& operator=(const rvector_file&) = delete;      // 禁止拷贝赋值
        rvector_file& operator=(rvector_file&&) = delete;           // 禁止移动赋值
    };
}

#endif // !RING_FILE_HPP