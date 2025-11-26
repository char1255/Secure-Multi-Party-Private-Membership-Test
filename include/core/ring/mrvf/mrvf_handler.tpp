#ifndef MRVF_HANDLER_TPP
#define MRVF_HANDLER_TPP

#include <cstring>
#include <fstream>

#include "core/mpmtcfg.hpp"
#include "core/ring/mrvf/mrvf_handler.hpp"
#include "core/crc/crc64.hpp"

namespace mpmt
{
    template<typename RT>
    mrvf_handler<RT>::mrvf_handler(const mrvf_handler::config& config)
        : mc_config(config)
    {}

    template<typename RT>
    uint8_t mrvf_handler<RT>::read_ring_size(const std::string& load_path)
    {
        // 1-新建文件
        std::ifstream in_file(load_path, std::ios::binary);
        if (!in_file)
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::exc_type::OPEN_ERROR,
                "Can not open the file [" + load_path + "]."
            );
        }


        // 2-偏移至 ring_size字段
        //  2.1-偏移输入流指针至offset
        const size_t offset = mc_BOF_BYTE_SIZE;
        in_file.seekg(offset);

        //  2.2-检查是否seek成功
        if (!in_file)
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::exc_type::INVALID_EOF,
                "The file ["
                + load_path
                + "] was unexpectedly truncated, making it impossible to access offset="
                + std::to_string(offset) + "."
            );
        }


        // 3-读取ring_size字段
        //  3.1-读取
        uint8_t ring_size;
        in_file.read((char*)&ring_size, sizeof(uint8_t));

        //  3.2-检查是否读入成功
        if (!in_file)
        {
            throw mpmt::mrvf_exc
            (
                mrvf_exc::exc_type::IFAILED,
                "Can not read file["
                + load_path
                + "] at offset="
                + std::to_string(offset)
                + "correctly."
            );
        }


        // 4-返回环大小
        return ring_size;
    }

    template<typename RT>
    mrvf<RT> mrvf_handler<RT>::load(const std::string& load_path)
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
            std::ifstream in_file(load_path, std::ios::binary);
            if (!in_file)
            {
                throw mpmt::mrvf_exc
                (
                    mrvf_exc::exc_type::OPEN_ERROR,
                    "Can not open the file [" + load_path + "]."
                );
            }


            // 2-完整读入文件
            //  2.1-获取完整的文件大小并重置指针
            in_file.seekg(0, std::ios::end);
            const size_t c_file_byte_size = static_cast<std::size_t>(in_file.tellg());
            in_file.seekg(0, std::ios::beg);

            //  2.2-分配缓存区
            std::unique_ptr<uint8_t[]> file_buffer = std::make_unique<uint8_t[]>(c_file_byte_size);

            //  2.3-读入数据到缓存区并校验操作
            in_file.read(reinterpret_cast<char*>(file_buffer.data()), c_file_byte_size);
            if (!in_file)
            {
                throw mpmt::mrvf_exc
                (
                    mrvf_exc::exc_type::IFAILED,
                    "Can not read all file["
                    + load_path
                    + "] correctly."
                );
            }


            // 3-从缓冲区读出数据
            //  3.1-初始化读指针
            size_t ipointer = 0ULL;

            //  3.2-读取文件头并校验
            for (size_t i = 0;i < mc_BOF_BYTE_SIZE;++i)
            {
                if (file_buffer[i + ipointer] != mc_BOF[i])
                {
                    throw mpmt::mrvf_exc
                    (
                        mrvf_exc::exc_type::INVALID_BOF,
                        "BOF mismatch found in file_buffer at index " + std::to_string(i) + "."
                    );
                }
            }
            ipointer += mc_BOF_BYTE_SIZE;

            //  3.3 读入环大小字段
            uint8_t ring_size = file_buffer[ipointer];
            if (ring_size != sizeof(RT))
            {
                throw mpmt::mrvf_exc
                (
                    mrvf_exc::exc_type::RING_SIZE_MISMATCH,
                    "The file["
                    + load_path
                    + "]declares ring as Z_{2^"
                    + std::to_string(ring_size * 8)
                    + "}, but the selected parameter is Z_{2^"
                    + std::to_string(sizeof(RT) * 8)
                    + "}."
                );
            }
            ipointer += mc_RING_SIZE_BYTE_SIZE;

            //  3.4-读入向量大小字段 
            size_t rvector_size;
            std::memcpy
            (
                reinterpret_cast<char*>(&rvector_size),
                reinterpret_cast<char*>(file_buffer.get()) + ipointer,
                mc_RVECTOR_SIZE_BYTE_SIZE
            );
            ipointer += mc_RVECTOR_SIZE_BYTE_SIZE;
            size_t rvector_byte_size = rvector_size * sizeof(RT);

            //  3.5 写入向量
            //  (1) 初始化 rvector
            mpmt::rvector<RT> l_rvector(rvector_size);
            //  (2) 将数据拷入rvector
            std::memcpy
            (
                reinterpret_cast<char*>(l_rvector.m_data),
                reinterpret_cast<char*>(file_buffer.get()) + ipointer,
                rvector_byte_size
            );
            ipointer += (rvector_byte_size);

            // 3.6 进行crc64校验
            if (mc_config.m_validate_checksum)
            {
                // (1) 读出校验和
                uint64_t file_crc64;
                std::memcpy
                (
                    reinterpret_cast<char*>(&file_crc64),
                    reinterpret_cast<char*>(file_buffer.get()) + ipointer,
                    mc_CRC64_BYTE_SIZE
                );
                // (2) 计算CRC校验长度
                const size_t c_crc64_compute_len
                    = mc_RING_SIZE_BYTE_SIZE
                    + mc_RVECTOR_SIZE_BYTE_SIZE
                    + rvector_byte_size;
                // (3) 比较CRC校验和
                uint64_t computing_crc64 = mpmt::crc64::compute
                (
                    file_buffer.get() + mc_BOF_BYTE_SIZE,
                    c_crc64_compute_len
                );
                if (file_crc64 != computing_crc64)
                {
                    throw mpmt::mrvf_exc
                    (
                        mrvf_exc::exc_type::CRC64_CHECK_FAILED,
                        "Checksum mismatch--Expected "
                        + std::to_string(file_crc64)
                        + " (from file) but computed "
                        + std::to_string(computing_crc64)
                        + "."
                    );
                }
            }
            ipointer += mc_CRC64_BYTE_SIZE;

            // 3.7 比较件尾
            for (size_t i = 0;i < mc_EOF_BYTE_SIZE;++i)
            {
                if (file_buffer[i + ipointer] != mc_EOF[i])
                {
                    throw mpmt::mrvf_exc
                    (
                        mrvf_exc::exc_type::INVALID_EOF,
                        "EOF mismatch found in file_buffer at index " + std::to_string(i) + "."
                    );
                }

            }
            ipointer += mc_EOF_BYTE_SIZE;

            // 3.8-防御性断言，确保没有未定义错误
            MPMT_ASSERT(ipointer == c_file_byte_size, "File write operation was unexpectedly interrupted.");


            // 4-返回读入的mrvf对象
            return mrvf<RT>(std::move(l_rvector));
        }
    }


    template<typename RT>
    void mrvf_handler<RT>::save
    (
        const std::string& save_path,
        const mrvf<RT>& mrvf_obj
    )
    {
        // 其实逻辑上根本不可能出现这种问题，mrvf<RT>构造时的ring_size参数都是sizeof(RT)，
        // 且ring_size是const，只是写一个防御性判断...
        if (mrvf_obj.mc_ring_size != sizeof(RT))
        {
            throw mpmt::mrvf_exc(
                mrvf_exc::exc_type::RING_SIZE_MISMATCH,
                "Ring size mismath while saving mrvf to file [" + save_path + "]."
            );
        }

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
                    mrvf_exc::exc_type::OPEN_ERROR,
                    "Can not open the new created file [" + save_path + "]."
                );
            }


            // 2-建立文件缓存区 
            //  2.1-计算常用长度
            //  (1) 向量长度
            const size_t c_rvector_size = mrvf_obj.m_rvector.size();
            //  (2) 向量在文件中的实际存储大小（字节）
            const size_t c_rvector_byte_size = c_rvector_size * sizeof(RT);
            //  (3) 文件大小（字节）
            const size_t c_file_byte_size           // 单位：字节
                = mc_BOF_BYTE_SIZE                  // 文件头
                + mc_RING_SIZE_BYTE_SIZE            // 环大小字段
                + mc_RVECTOR_SIZE_BYTE_SIZE         // 向量大小字段
                + c_rvector_byte_size               // 数据段
                + mc_CRC64_BYTE_SIZE                // CRC64校验码字段
                + mc_EOF_BYTE_SIZE;                 // 文件尾

            //  2.2-为缓存区分配内存
            std::unique_ptr<uint8_t[]> file_buffer = std::make_unique<uint8_t[]>(c_file_byte_size);


            // 3-向缓冲区写入文件
            //  3.1-初始化写指针
            size_t opointer = 0ULL;

            //  3.2-写入文件头
            for (size_t i = 0;i < mc_BOF_BYTE_SIZE;++i)
            {
                file_buffer[i + opointer] = mc_BOF[i];
            }
            opointer += mc_BOF_BYTE_SIZE;

            //  3.3 写入环大小字段
            file_buffer[opointer] = mrvf_obj.mc_ring_size;
            opointer += mc_RING_SIZE_BYTE_SIZE;

            //  3.4 写入向量大小
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer.get()) + opointer,
                reinterpret_cast<char*>(&c_rvector_size),
                mc_RVECTOR_SIZE_BYTE_SIZE
            );
            opointer += mc_RVECTOR_SIZE_BYTE_SIZE;

            //  3.5 写入向量
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer.get()) + opointer,
                reinterpret_cast<char*>(mrvf_obj.m_rvector.m_data),
                c_rvector_byte_size
            );
            opointer += (c_rvector_byte_size);

            // 3.6 进行crc64校验码计算，并写入文件
            // (1) 计算CRC校验长度
            const size_t c_crc64_compute_len
                = mc_RING_SIZE_BYTE_SIZE
                + mc_RVECTOR_SIZE_BYTE_SIZE
                + c_rvector_byte_size;
            // (2) 计算CRC校验和
            uint64_t computing_crc64 = mpmt::crc64::compute
            (
                file_buffer.get() + mc_BOF_BYTE_SIZE,
                c_crc64_compute_len
            );
            // (3) 写入校验和
            std::memcpy
            (
                reinterpret_cast<char*>(file_buffer.get()) + opointer,
                reinterpret_cast<char*>(&computing_crc64),
                mc_CRC64_BYTE_SIZE
            );
            opointer += mc_CRC64_BYTE_SIZE;

            // 3.7 写入文件尾
            for (size_t i = 0;i < mc_EOF_BYTE_SIZE;++i)
            {
                file_buffer[i + opointer] = mc_EOF[i];
            }
            opointer += mc_EOF_BYTE_SIZE;

            // 3.8-防御性断言，确保没有未定义错误
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