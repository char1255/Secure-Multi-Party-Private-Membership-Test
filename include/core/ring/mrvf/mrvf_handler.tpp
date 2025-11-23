#ifndef MRVF_HANDLER_TPP
#define MRVF_HANDLER_TPP

#include <cstring>
#include <fstream>

#include "core/mpmtcfg.hpp"
#include "core/ring/mrvf/mrvf_handler.hpp"

namespace mpmt
{
    template<typename RT>
    mrvf_handler<RT>::mrvf_handler(const mrvf_handler::config& config)
        : mc_config(config)
    {}

    template<typename RT>
    void mrvf_handler<RT>::save
    (
        const std::string& save_path,
        const mrvf<RT>& mrvf_obj
    )
    {
        if (mc_config.m_use_memory_map)
        {
#ifdef MPMT_OS_WIN
#elif MPMT_OS_IOS
#error "No immediate plan."
#elif MPMT_OS_MACOS
#error "Coming soon."
#elif MPMT_OS_ANDROID
#error "No immediate plan."
#elif MPMT_OS_LINUX
#error "Coming soon."
#endif 
        }
        else
        {
            // 1-新建文件
            std::ofstream out_file(save_path, std::ios::binary);
            if (!out_file)
            {
                throw mpmt::mrvf_exc
                (
                    mrvf_exc::exc_type::FILE_OPEN_ERROR,
                    "Can not open the new created file [" + save_path + "]."
                );
            }


            // 2-建立文件缓存区 
            //  2.1-计算常用长度
            //  (1) 向量长度    // 这个有大问题，涉及size_t尺寸以及与uint32_t的对齐
            const size_t c_rvector_size = mrvf_obj.m_rvector.size();
            //  (2) 向量在文件中的实际存储大小（字节）
            const size_t c_rvector_byte_size = c_rvector_size * sizeof(RT);
            //  (3) 文件大小（字节）
            const size_t c_file_byte_size           // 单位：字节
                = mc_BOF_BYTE_SIZE                  // 文件头
                + mc_CRC64_STANDARD_BYTE_SIZE       // CRC64计算标准字段
                + mc_RING_SIZE_BYTE_SIZE            // 环大小字段
                + mc_RVECTOR_SIZE_BYTE_SIZE         // 向量大小字段
                + c_rvector_byte_size               // 数据段
                + mc_CRC64_BYTE_SIZE                // CRC64校验码字段
                + mc_EOF_BYTE_SIZE;                 // 文件尾

            //  2.2-为缓存区分配内存
            std::unique_ptr<uint8_t[]> file_buffer = std::make_unique<uint8_t[]>(c_file_byte_size);


            // 3-向缓冲区写入文件
            //  3.1-初始化写指针
            size_t opointer = 0;

            //  3.2-写入文件头
            for (size_t i = 0;i < mc_BOF_BYTE_SIZE;++i)
            {
                file_buffer[i + opointer] = mc_BOF[i];
            }
            opointer += mc_BOF_BYTE_SIZE;

            //  3.3-写入crc64计算标准字段
            auto standard_siter = mc_CRC64_STANDARD_TO_UINT8_T.find(mrvf_obj.mc_crc64_standard);
            if (standard_siter == mc_CRC64_STANDARD_TO_UINT8_T.end())
            {
                throw mpmt::mrvf_exc
                (
                    mrvf_exc::exc_type::UNDEFINED_CRC64_STANDARD,
                    "Key crc64_factory::standard="
                    + std::to_string(static_cast<int>(mrvf_obj.mc_crc64_standard))
                    + " is not found in std::unordered_map mc_CRC64_STANDARD_TO_UINT8_T."
                );
            }
            file_buffer[opointer] = standard_siter->second;
            opointer += mc_CRC64_STANDARD_BYTE_SIZE;

            //  3.4 写入环大小字段
            file_buffer[opointer] = mrvf_obj.mc_ring_size;
            opointer += mc_RING_SIZE_BYTE_SIZE;

            //  3.5 写入向量大小
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer.get()) + opointer,
                reinterpret_cast<char*>(&c_rvector_size),
                mc_RVECTOR_SIZE_BYTE_SIZE
            );
            opointer += mc_RVECTOR_SIZE_BYTE_SIZE;

            //  3.6 写入向量
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer.get()) + opointer,
                reinterpret_cast<char*>(mrvf_obj.m_rvector.data()),
                c_rvector_byte_size
            );
            opointer += (c_rvector_byte_size);

            // 3.7 进行crc64校验码计算，并写入文件
            // (1) 实例化接口
            std::unique_ptr<crc64> crc64_interface = crc64_factory::acquire_interface
            (
                mrvf_obj.mc_crc64_standard
            );
            // (2) 计算CRC校验长度
            const size_t c_crc64_compute_len
                = mc_CRC64_STANDARD_BYTE_SIZE
                + mc_RING_SIZE_BYTE_SIZE
                + mc_RVECTOR_SIZE_BYTE_SIZE
                + c_rvector_byte_size;
            // (3) 计算CRC校验和
            uint64_t crc64_checksum = crc64_interface->compute
            (
                file_buffer.get() + mc_BOF_BYTE_SIZE,
                c_crc64_compute_len
            );
            // (4) 写入校验和
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer.get()) + opointer,
                reinterpret_cast<char*>(&crc64_checksum),
                mc_CRC64_BYTE_SIZE
            );
            opointer += mc_CRC64_BYTE_SIZE;

            // 3.8 写入文件尾
            for (size_t i = 0;i < mc_EOF_BYTE_SIZE;++i)
            {
                file_buffer[i + opointer] = mc_EOF[i];
            }
            opointer += mc_EOF_BYTE_SIZE;

            // 3.9-防御性断言，确保没有未定义错误
            MPMT_ASSERT(opointer == c_file_byte_size, "File write operation was unexpectedly interrupted.");


            // 4-将缓存区内容写入文件
            out_file.write
            (
                reinterpret_cast<const char*>(file_buffer.get()),
                c_file_byte_size
            );
        }
    }
}
#endif // !MRVF_HANDLER_TPP