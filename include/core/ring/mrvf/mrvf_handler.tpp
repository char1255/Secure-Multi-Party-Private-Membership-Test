#ifndef MRVF_HANDLER_TPP
#define MRVF_HANDLER_TPP

#include <cstring>
#include <fstream>

#include "core/mpmtcfg.hpp"
#include "core/ring/mrvf/mrvf_handler.hpp"

namespace mpmt
{
    template<typename T>
    mrvf_handler<T>::mrvf_handler
    (
        const std::string& load_path,
        const crc64_factory::standard& crc64_standard,
        bool use_memory_mapping
    ) :
        m_is_checked_out(false),            // 初始化状态下，缓存未被借出
        m_use_memory_map(use_memory_map)    // 是否使用内存映射
    {
        if (use_memory_mapping)
        {
#ifdef MPMT_OS_WIN
#elif MPMT_OS_IOS
#error "No immediate plan."
#elif MPMT_OS_MACOS
#error "Coming soon."
#elif MPMT_OS_ANDROID
#error "No immediate plan."
#elif MPMT_OS_HARMONY
#error "No immediate plan."
#elif MPMT_OS_LINUX
#error "Coming soon."
#endif 
        }

    }

    template<typename T>
    void mrvf_handler<T>::save
    (
        const std::string& save_path,
        const crc64_factory::standard& crc64_standard,
        bool use_memory_mapping
    )
    {
        // 保障 rvector segement不为空
        MPMT_ASSERT(!m_is_checked_out, "rvector segment 使用权尚未check in，无法执行写入。");

        if (use_memory_mapping)
        {
#ifdef MPMT_OS_WIN
#elif MPMT_OS_IOS
#error "No immediate plan."
#elif MPMT_OS_MACOS
#error "Coming soon."
#elif MPMT_OS_ANDROID
#error "No immediate plan."
#elif MPMT_OS_HARMONY
#error "No immediate plan."
#elif MPMT_OS_LINUX
#error "Coming soon."
#endif 
        }
        else
        {
            // 新建文件
            std::ofstream out_file(save_path, std::ios::binary);
            if (!out_file)
            {
                throw mpmt::mrvf_exc
                (
                    mrvf_exc::exc_type::FILE_OPEN_ERROR,
                    "Can not open the new created file [" + save_path + "]."
                );
            }

            // 计算文件大小(字节数)
            const size_t file_size
                = c_BOF_LEN                     // 文件头长度
                + c_VERSION_LEN                 // 版本号字段长度
                + c_CRC64_STANDARD_LEN          // CRC64计算标准字段长度
                + c_RING_SIZE_LEN               // 环大小字段长度
                + c_RVECTOR_SIZE_LEN            // 向量大小字段长度
                + m_rvector_size * sizeof(T)    // 数据载荷字节数 = 向量大小*环大小
                + c_CRC64_LEN                   // 校验字段长度
                + c_EOF_LEN;                    // 文件尾长度

            // 新建缓存区
            std::unique_ptr<uint8_t[]> file_buffer = std::make_unique<uint8_t[]>(file_size);
            uint8_t* const file_buffer_raw = file_buffer.get();

            // 初始化写指针
            size_t ipointer = 0ULL;

            // 向缓存区写入文件头
            for (size_t i = 0;i < c_BOF_LEN;++i)
            {
                file_buffer_raw[i + ipointer] = c_BOF[i];
            }
            ipointer += c_BOF_LEN;

            // 向缓存区写入文件版本号
            file_buffer_raw[ipointer] = c_VERSION;
            ipointer += c_VERSION_LEN;

            // 向缓存区写入指定的CRC64计算标准
            file_buffer_raw[ipointer] = m_crc64_standard;
            ipointer += c_CRC64_STANDARD_LEN;

            // 向缓存区写入环大小
            file_buffer_raw[ipointer] = m_ring_size;
            ipointer += c_RING_SIZE_LEN;

            // 写入向量大小（数据段长度）
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer_raw) + ipointer,
                reinterpret_cast<char*>(&m_rvector_size),
                c_RVECTOR_SIZE_LEN
            );
            ipointer += c_RVECTOR_SIZE_LEN;

            // 写入向量数据
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer_raw) + ipointer,
                reinterpret_cast<char*>(m_rvector_segment.get()),
                m_rvector_size * sizeof(T)
            );
            ipointer += (m_rvector_size * sizeof(T));

            // 实例化crc计算接口
            std::unique_ptr<crc64> crc64_interface = crc64_factory::acquire_interface
            (
                c_UINT8_T_TO_CRC64_STANDARD(m_crc64_standard)
            );

            // 计算CRC64校验码
            const size_t crc64_compute_len
                = c_VERSION_LEN
                + c_CRC64_STANDARD_LEN
                + c_RING_SIZE_LEN
                + c_RVECTOR_SIZE_LEN
                + m_rvector_size * sizeof(T);

            uint64_t crc64_checksum = crc64_interface->compute
            (
                file_buffer_raw + c_BOF_LEN,
                crc64_compute_len
            );

            // 写入CRC64校验码
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer_raw) + ipointer,
                reinterpret_cast<char*>(&crc64_checksum),
                c_CRC64_LEN
            );
            ipointer += c_CRC64_LEN;

            // 向缓存区写入文件尾
            for (size_t i = 0;i < c_EOF_LEN;++i)
            {
                file_buffer_raw[i + ipointer] = c_EOF[i];
            }
            ipointer += c_EOF_LEN;

            // 一般断言是没问题的
            MPMT_ASSERT(ipointer == file_size, "File write operation was unexpectedly interrupted.");

            // 写入文件
            out_file.write
            (
                reinterpret_cast<const char*>(file_buffer_raw),
                file_size
            );
        }
    }

    template<typename T>
    mrvf_handler<T>::~mrvf_handler()
    {
        MPMT_ASSERT(!m_is_checked_out, "rvector buffer 使用权尚未check in就析构mrvf_handler对象。");
    }

    template<typename T>
    T* mrvf_handler<T>::rvector_segment_check_out()
    {
        MPMT_ASSERT(!m_is_checked_out, "rvector buffer 使用权已check out。");
        m_is_checked_out = true;
        return m_rvector_segment.release();
    }

    template<typename T>
    void mrvf_handler<T>::rvector_segment_check_in(T* const buffer)
    {
        MPMT_ASSERT(m_is_checked_out, "rvector buffer 使用权已check in。");
        m_rvector_segment.reset(buffer);
        m_is_checked_out = false;
    }

    template<typename T>
    size_t mrvf_handler<T>::rvector_size()
    {
        return static_cast<size_t>(m_rvector_size);
    }
}
#endif // !MRVF_HANDLER_TPP