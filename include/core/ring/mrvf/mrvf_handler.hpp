#ifndef MRVF_HANDLER_HPP
#define MRVF_HANDLER_HPP

#include <string>
#include <memory>
#include <fstream>

#include "core/ring/ring.hpp"
#include "core/exception/mrvf_exc.hpp"
#include "core/crc/crc64.hpp"
#include "core/crc/crc64_ecma182.hpp"

template<typename T>
static inline T swap_uint(T val)
{
    static_assert(
        std::is_same_v<T, uint16_t> ||
        std::is_same_v<T, uint32_t> ||
        std::is_same_v<T, uint64_t>,
        "T must be uint16_t, uint32_t, or uint64_t."
        );

    if constexpr (std::is_same_v<T, uint16_t>)
    {
        return __builtin_bswap16(val);
    }
    else if constexpr (std::is_same_v<T, uint32_t>)
    {
        return __builtin_bswap32(val);
    }
    else
    {
        return __builtin_bswap64(val);
    }
}

/** @namespace 项目命名空间 */
namespace mpmt
{
    template<typename T>
    class mrvf_handler
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
        explicit mrvf_handler
        (
            const std::string& load_path,
            const bool use_memory_mapping = false
        );

        /** @brief 构造函数 */
        void save(const std::string& save_path)
        {
            // 定义crc计算接口
            std::unique_ptr<mpmt::crc64> crc = std::make_unique<mpmt::crc64_ecma182>();

            // 新建文件
            // 写入文件头
            // 写入1字节，表示环大小，即 m_ring_size
            // 写入8字节，表示文件长度，即 m_size
            // 根据m_size，写入m_size个T数据
            // 计算数据段+环大小+文件长这一段的CRC，并写入八字节CRC
            // 写入文件尾
        }

        std::unique_ptr<T[]> release_buffer()
        {
            return std::move(this->m_buffer);
        }

        /** @brief 析构函数 */
        ~mrvf_handler();


    private:
        const bool m_use_memory_mapping;                                // 是否使用内存映像
        uint8_t m_ring_size;                                            // 环大小：环位宽
        uint64_t m_data_size;                                           // 向量大小：m_data_size * sizeof(T) (bytes)
        std::unique_ptr<T[]> m_buffer;                                  // 数据载荷段

        /** @brief constant */
        static inline constexpr uint64_t M_BOF = 0x4D5256465F424F46;    // 文件头-标识"MRVF_BOF"
        static inline constexpr uint64_t M_EOF = 0x4D5256465F454F46;    // 文件尾-标识"MRVF_EOF"
        static inline constexpr std::string M_FILE_EXTENSION = ".mrvf"; // 文件t拓展名

        /** @brief 禁用拷贝与移动操作 */
        mrvf_handler() = delete;                                    // 禁止默认构造
        mrvf_handler(const mrvf_handler&) = delete;                 // 禁止拷贝构造
        mrvf_handler(mrvf_handler&&) = delete;                      // 禁止移动构造
        mrvf_handler& operator=(const mrvf_handler&) = delete;      // 禁止拷贝赋值
        mrvf_handler& operator=(mrvf_handler&&) = delete;           // 禁止移动赋值
    };
}

#include "core/ring/mrvf/mrvf_handler.tpp"

#endif // !MRVF_HANDLER_HPP