#ifndef MRVF_HANDLER_TPP
#define MRVF_HANDLER_TPP
#include "core/mpmtcfg.hpp"
#include "core/ring/mrvf/mrvf_handler.hpp"

namespace mpmt
{
    template<typename T>
    mrvf_handler<T>::mrvf_handler
    (
        const std::string& load_path,
        bool use_memory_mapping
    ) :
        m_is_checked_out(false),            // 初始化状态下，缓存未被借出
        m_use_memory_map(use_memory_map)    // 是否使用内存映射
    {
        if(use_memory_mapping)
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

        // RAII 文件配置
        std::ifstream in_file(load_path, std::ios::binary);
        if (!in_file)
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::error_type::FILE_OPEN_ERROR,
                "Can not open file [" + load_path + "]"
            );
        }

        // 读取文件头
        uint64_t fBOF;              // 文件头magic number
        uint64_t reversed_fBOF;     // 调整端序后的文件头
        bool is_little_endian;      // 判断当前系统为大端还是小端
        in_file.read(reinterpret_cast<char*>(&fBOF), sizeof(uint64_t));
        reversed_fBOF = swap_endian<uint64_t>(fBOF);
        if (fBOF == this->M_BOF)
        {
            // 文件默认为小端，则系统为小端
            is_little_endian = true;
        }
        else if (reversed_fBOF == this->M_BOF)
        {
            // 文件默认为小端，则系统为大端
            is_little_endian = false;
        }
        else
        {
            // 文件头不匹配，抛出错误
            throw mpmt::mrvf_exc
            (
                mrvf_exc::error_type::FILE_INVALID_BOF,
                "Invalid file header (BOF) [" + load_path + "]"
            )
        }

        // 读取版本
        size_t offset = 8;  // 文件指针指向'version'字段
        in_file.seekg(offset);
        if (!in_file)
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::error_type::FILE_SEEK_ERROR,
                "Can not seek file ["
                + load_path
                + "] at offset="
                + std::to_string((offset))
                + "."
            );
        }
        in_file.read(reinterpret_cast<char*>(&m_version), sizeof(uint8_t));

        // 读取环大小
        uint8_t m_ring_size;
        offset += 1; // 文件指针指向 'ring_size'字段 
        in_file.seekg(offset);
        if (!in_file)
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::error_type::FILE_SEEK_ERROR,
                "Can not seek file ["
                + load_path
                + "] at offset="
                + std::to_string((offset))
                + "."
            );
        }
        in_file.read(reinterpret_cast<char*>(&m_ring_size), sizeof(uint8_t));
        if (m_ring_size != sizeof(T))
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::error_type::FILE_RING_SIZE_MISMATCH,
                "File [" + load_path
                + "] specifies a ring \mathbb{Z}_{2^"
                + std::to_string(m_ring_size) +
                "} and cannot be loaded via mrvf_handler<ring"
                + std::to_string(8 * sizeof(T))
                + ">."
            );
        }

        // 读取向量大小
        offset += 1; // 文件指针指向 'rvector_size'字段 
        in_file.seekg(offset);
        if (!in_file)
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::error_type::FILE_SEEK_ERROR,
                "Can not seek file ["
                + load_path
                + "] at offset="
                + std::to_string((offset))
                + "."
            );
        }
        in_file.read(reinterpret_cast<char*>(&m_rvector_size), sizeof(uint32_t));

        // 读取向量
        offset += 4; // 文件指针指向 'rvector_segment'字段 
        in_file.seekg(offset);
        m_rvector_buf = std::make_unique<T[]>(m_rvector_size);
        in_file.read(reinterpret_cast<char*>(m_rvector_buf.get()), m_rvector_size * sizeof(T));
        if (!is_little_endian)  // 如果是大端序，则调整向量的端序
        {
            /**
             * 这里其实考虑：如果端序调整的开销过高，则需要在文件中标注端序，文件跨系统运行时，
             * 需要检测计算机端序，并调整文件端序，此后便无需调整。
             * 这里暂时先默认文件为小端序写入的，等后面profiling一下调整端序的开销
             */
            for (size_t i = 0;i < m_rvector_size;++i)
            {
                swap_endian<T>(m_rvector_buf[i]);
            }
        }

        // 读取CRC
        uint64_t expected_crc64;   // 从文件中读出来的crc值
        uint64_t computed_crc64;   // 校验计算得到的crc值
        offset += m_rvector_size * sizeof(T);   // 指针指向文件校验码
        in_file.seekg(offset);
        if (!in_file)
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::error_type::FILE_SEEK_ERROR,
                "Can not seek file ["
                + load_path
                + "] at offset="
                + std::to_string(offset)
                + "."
            );
        }
        in_file.read(reinterpret_cast<char*>(&expected_crc64), sizeof(uint64_t));

        // 实例化CRC计算接口，并计算CRC64校验值
        std::unique_ptr<mpmt::crc64> crc = std::make_unique<mpmt::crc64_ecma182>();

        // ...待实现计算校验
        // ... 

        // 读入文件尾
        uint64_t fEOF;  // 文件尾magic number
        offset += 8;    // 指向文件尾
        in_file.seekg(1);
        in_file.read(reinterpret_cast<char*>(&fEOF), sizeof(uint64_t));
        if (!is_little_endian)
        {
            swap_endian<uint64_t>(fEOF);
        }
        if (fEOF != this->M_EOF)
        {  // 文件尾不匹配，抛出错误
            throw mpmt::mrvf_exc
            (
                mrvf_exc::error_type::FILE_INVALID_EOF,
                "Invalid file footer (EOF) [" + load_path + "]"
            )
        }
    }

    template<typename T>
    void mrvf_handler<T>::save(const std::string& save_path)
    {
        // 实例化crc计算接口
        std::unique_ptr<mpmt::crc64> crc = std::make_unique<mpmt::crc64_ecma182>();

        // 新建文件
        // 写入文件头
        // 写入1字节，表示环大小，即 m_ring_size
        // 写入8字节，表示文件长度，即 m_size
        // 根据m_size，写入m_size个T数据
        // 计算数据段+环大小+文件长这一段的CRC，并写入八字节CRC
        // 写入文件尾
    }

    template<typename T>
    mrvf_handler<T>::~mrvf_handler(){
        MPMT_ASSERT(!m_is_checked_out, "对于对象 rvector buffer 使用权并未归还。")
    }

    template<typename T>
    T* mrvf_handler<T>::rvector_buffer_check_out(){
        m_is_checked_out = true;
        return m_rvector_buffer.release();
    }

    template<typename T>
    void mrvf_handler<T>::rvector_buffer_check_in(T* const buffer){
        m_rvector_buffer.reset(buffer);
        m_is_checked_out = false;
    }




}
#endif // !MRVF_HANDLER_TPP