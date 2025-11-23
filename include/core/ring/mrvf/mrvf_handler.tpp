#ifndef MRVF_HANDLER_TPP
#define MRVF_HANDLER_TPP

#include <cstring>
#include <fstream>

#include "core/mpmtcfg.hpp"
#include "core/ring/mrvf/mrvf_handler.hpp"

namespace mpmt
{
    template<typename RT>
    mrvf_handler<RT>::mrvf_handler(const mrvf_handler::config& config) {}

    template<typename RT>
    void mrvf_handler<RT>::save
    (
        const std::string& save_path,
        const mrvf<RT>& mrvf_obj
    )
    {
        if (c_config.use_memory_map)
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

            const size_t file_size 
                = c_BOF_LEN
                + c_VERSION_LEN
                + c_CRC64_STANDARD_LEN
                + c_RING_SIZE_LEN
                + c_RVECTOR_SIZE_LEN
                + 
                ;

            // 新建缓存区
            std::unique_ptr<uint8_t[]> file_buffer = std::make_unique<uint8_t[]>(file_size);
            uint8_t* const file_buffer_raw = file_buffer.get();
        }
    }
}
#endif // !MRVF_HANDLER_TPP