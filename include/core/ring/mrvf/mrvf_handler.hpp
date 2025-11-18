#ifndef MRVF_HANDLER_HPP
#define MRVF_HANDLER_HPP

#include <string>
#include <memory>
#include <fstream>

#include "core/ring/ring.hpp"
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
        explicit mrvf_handler(const std::string& load_path);

        /** @brief 保存函数 */
        void save(const std::string& save_path);

        /** @brief 析构函数 */
        ~mrvf_handler();

        /** @brief 暴露只读迭代器接口 */
        class const_iterator
        {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

        private:
            const T* cur_ptr;   // 当前指针

            // 默认构造函数
            const_iterator();

            // 初始化构造函数
            explicit const_iterator(const T* ptr) : current_ptr(ptr) {}

            // 解引用操作符 (*iter) -> 只读
            reference operator*() const;

            // 成员访问操作符 (iter->member) -> 只读
            pointer operator->() const;

            // 前置递增 (++iter)
            const_iterator& operator++();

            // 后置递增 (iter++)
            // C++17 InputIterator 只需要单通道性，但为了语义还是实现它
            const_iterator operator++(int)
            {
                const_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            // 相等操作符
            friend bool operator==(const const_iterator& a, const const_iterator& b);

            // 不等操作符
            friend bool operator!=(const const_iterator& a, const const_iterator& b);
        };

    private:
        const bool m_use_memory_map;                                    // 是否使用内存映像
        uint8_t m_version;                                              // 文件版本号
        uint8_t m_ring_size;                                            // 环大小：环位宽
        uint32_t m_rvector_size;                                        // 向量大小：m_rvector_size * sizeof(T) (bytes)
        T* const m_file_buffer;                                         // 文件指针

        /** @brief constant */
        static inline constexpr uint64_t M_BOF = 0x4D5256465F424F46;    // 文件头-标识"MRVF_BOF"
        static inline constexpr uint64_t M_EOF = 0x4D5256465F454F46;    // 文件尾-标识"MRVF_EOF"
        static inline constexpr std::string M_FILE_EXTENSION = ".mrvf"; // 文件t拓展名

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