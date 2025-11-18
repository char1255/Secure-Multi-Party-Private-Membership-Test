#ifndef MRVF_HANDLER_HPP
#define MRVF_HANDLER_HPP

#include <string>
#include <memory>
#include <fstream>

#include "core/ring/ring.hpp"
#include "core/ring/rvector.hpp"
#include "core/exception/mrvf_exc.hpp"
#include "core/crc/crc64.hpp"
#include "core/crc/crc64_ecma182.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    template<typename T>
    class mrvf_handler
    {
    public:
        /** @brief 声明rvector为友元类 */
        friend class rvector<T>;

        /** @brief 断言限制模板类型 */
        static_assert(
            std::is_same_v<T, ring1> ||
            std::is_same_v<T, ring8> ||
            std::is_same_v<T, ring16> ||
            std::is_same_v<T, ring32> ||
            std::is_same_v<T, ring64>,
            "T must be ring1, ring8, ring16, ring32 or ring64."
            );

        /** @brief 唯一允许的构造函数 */
        explicit mrvf_handler
        (
            const std::string& load_path, 
            bool use_memory_mapping = false
        );

        /** @brief 保存函数 */
        void save(const std::string& save_path);

        /** @brief 析构函数 */
        ~mrvf_handler();

    protected:
        /** @brief 友元类可以申请对向量缓存区的读写权限 */
        T* rvector_buffer_check_out();                                  // 将向量缓存区的读写权限签发
        void rvector_buffer_check_in(T* const buffer);                  // 收回向量缓存区的读写权限

    private:
        /** @brief member variable */
        bool m_is_checked_out;                                          // 
        const bool m_use_memory_map;                                    // 是否使用内存映像
        uint8_t m_version;                                              // 文件版本号
        uint8_t m_ring_size;                                            // 环大小：环位宽
        uint32_t m_rvector_size;                                        // 向量大小：m_rvector_size * sizeof(T) (bytes)
        std::unique_ptr<T[]> m_rvector_buffer;                          // 向量缓存指针

        /** @brief mrcf file format size */
        static constexpr size_t M_BOF_LEN = 8;                          // 文件头长度
        static constexpr size_t M_VERSION_LEN = 1;                      // 版本号字段长度
        static constexpr size_t M_RING_SIZE_LEN = 1;                    // 环大小字段长度
        static constexpr size_t M_RVECTOR_SIZE_LEN = 4;                 // 向量大小字段长度
        static constexpr size_t M_CRC64_LEN = 8;                        // CRC64字段长度
        static constexpr size_t M_EOF_LEN = 8;                          // 文件尾长度

        /** @brief constant value */
        static constexpr uint64_t M_BOF = 0x4D5256465F424F46;           // 文件头-标识"MRVF_BOF"
        static constexpr uint64_t M_EOF = 0x4D5256465F454F46;           // 文件尾-标识"MRVF_EOF"
        static constexpr std::string M_FILE_EXTENSION = ".mrvf";        // 文件拓展名

        /** @brief 禁用拷贝与移动操作 */
        mrvf_handler() = delete;                                        // 禁止默认构造
        mrvf_handler(const mrvf_handler&) = delete;                     // 禁止拷贝构造
        mrvf_handler(mrvf_handler&&) = delete;                          // 禁止移动构造
        mrvf_handler& operator=(const mrvf_handler&) = delete;          // 禁止拷贝赋值
        mrvf_handler& operator=(mrvf_handler&&) = delete;               // 禁止移动赋值

    };

    template<>
    class mrvf_handler<ring1>
    {
        /** @brief 唯一允许的构造函数 */
        explicit mrvf_handler(const std::string& load_path);

        /** @brief 保存函数 */
        void save(const std::string& save_path);

        /** @brief 析构函数 */
        ~mrvf_handler();
    };
}

#include "core/ring/mrvf/mrvf_handler.tpp"

#endif // !MRVF_HANDLER_HPP