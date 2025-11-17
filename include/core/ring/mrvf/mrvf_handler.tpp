#ifndef MRVF_HANDLER_TPP
#define MRVF_HANDLER_TPP

#include "core/ring/mrvf/mrvf_handler.hpp"

namespace mpmt
{
    template<typename T>
    mrvf_handler<T>::mrvf_handler(
        const std::string& load_path,
        const bool use_memory_mapping
    ) :
        m_use_memory_mapping(use_memory_mapping)
    {
        // 定义crc计算接口
        std::unique_ptr<mpmt::crc64> crc = std::make_unique<mpmt::crc64_ecma182>();

        // 使用内存映像
        if (m_use_memory_mapping)
        {
#ifdef defined(MPMT_OS_WIN)

#elif defined(MPMT_OS_IOS)

#elif defined(MPMT_OS_MACOS)

#elif defined(MPMT_OS_ANDROID)

#elif defined(MPMT_OS_LINUX)

#endif 
        }
        // 常规文件读取 
        else
        {
            std::ifstream in_file(load_path, std::ios::binary);
            if (!in_file)
            {
                throw mpmt::mrvf_exc(
                    mrvf_exc::error_type::FILE_OPEN_ERROR,
                    "Can not open file: " + load_path
                );
            }

            in_file.seekg();
        }

        // 检查八字节头是否为MPMT
        // 读入1字节ring_size
        // 读入八字节：文件长度， 暂时限定为2^35最大，保留2^64
        // 顺次读入数据
        // 读入八字节CRC, 并校验CRC是否正确
        // 检查文件尾
        // 你无需关注其他情况，我自己会处理，你先写一个示例
    }


}
#endif // !MRVF_HANDLER_TPP