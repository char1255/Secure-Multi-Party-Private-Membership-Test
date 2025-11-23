#ifndef MRVF_HANDLER_HPP
#define MRVF_HANDLER_HPP

#include <string>
#include <memory>
#include <unordered_map>

#include "core/ring/ring.hpp"
#include "core/ring/rvector.hpp"
#include "core/exception/mrvf_exc.hpp"
#include "core/crc/crc64.hpp"
#include "core/crc/crc64_factory.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    // mrvf结构体，一般不修改版本号选项
    // 在进行环大小转换后，需要修改
    template<typename RT>
    struct mrvf
    {
        static_assert(
            is_ring_type,
            "RT must be ring1, ring8, ring16, ring32 or ring64."
            );

        mpmt::rvector<RT> m_rvector;    // 向量
        uint8_t c_version;              // 文件版本号
        uint8_t c_crc64_standard;       // crc64计算标准
        uint8_t c_ring_size;            // 环位宽
        uint32_t c_rvector_size;        // 向量元素数量
    };

    template<typename RT>
    class mrvf_handler
    {
    public:
        struct config
        {
            bool use_memory_map;        // 读写文件时是否启用内存映射
            bool validate_checksum;     // 读取文件时是否验证校验码
        };

        /** @brief 断言限制模板类型 */
        static_assert(
            is_ring_type,
            "RT must be ring1, ring8, ring16, ring32 or ring64."
            );

        mrvf_handler(const mrvf_handler::config& config);

        /**
         * @brief 从文件中载入mrvf对象
         * @param const std::string& load_path  加载路径
         * @return mrvf<RT>
         */
        mrvf<RT> load(const std::string& load_path);

        /**
         * @brief  保存mrvf对象为文件
         * @param  const std::string& save_path,
         * @param  const mrvf<RT>& mrvf_obj
         * @return void
         */
        void save
        (
            const std::string& save_path,
            const mrvf<RT>& mrvf_obj
        );

        ~mrvf_handler();

    private:
        const mrvf_handler::config c_config;                            // 加载、保存配置设置

        /** @brief mrcf file format size */
        static constexpr size_t c_BOF_LEN = 8ULL;                       // 文件头长度
        static constexpr size_t c_VERSION_LEN = 1ULL;                   // 版本号字段长度
        static constexpr size_t c_CRC64_STANDARD_LEN = 1ULL;            // CRC64计算标准字段长度
        static constexpr size_t c_RING_SIZE_LEN = 1ULL;                 // 环大小字段长度
        static constexpr size_t c_RVECTOR_SIZE_LEN = 4ULL;              // 向量大小字段长度
        static constexpr size_t c_CRC64_LEN = 8ULL;                     // CRC64字段长度
        static constexpr size_t c_EOF_LEN = 8ULL;                       // 文件尾长度

        /** @brief constant value */
        static constexpr uint8_t c_BOF[c_BOF_LEN] =                     // 文件头-标识"MRVF_BOF"
        {
            0x4d,0x52,0x56,0x46,0x5f,0x42,0x4f,0x46
        };
        static constexpr uint8_t c_EOF[c_EOF_LEN] =                     // 文件尾-标识"MRVF_EOF"
        {
            0x4d,0x52,0x56,0x46,0x5f,0x45,0x4f,0x46
        };
        static constexpr uint8_t c_VERSION = 0x01;                      // 文件版本

        static const inline std::string c_FILE_EXTENSION = ".mrvf";     // 文件拓展名

        static const inline std::unordered_map <                        // enum到字段值的健壮转换
            crc64_factory::standard,
            uint8_t
        > c_CRC64_STANDARD_TO_UINT8_T =
        {
            { crc64_factory::standard::ECMA182 , 0x00 },
            { crc64_factory::standard::ISO     , 0x01 }
        };

        static const inline std::unordered_map <                        // 字段值到enum的健壮转换
            uint8_t,
            crc64_factory::standard
        > c_UINT8_T_TO_CRC64_STANDARD =
        {
            { 0x00, crc64_factory::standard::ECMA182  },
            { 0x01, crc64_factory::standard::ISO      }
        };

        /** @brief 禁用拷贝与移动操作 */
        mrvf_handler() = delete;                                        // 禁止默认构造
        mrvf_handler(const mrvf_handler&) = delete;                     // 禁止拷贝构造
        mrvf_handler(mrvf_handler&&) = delete;                          // 禁止移动构造
        mrvf_handler& operator=(const mrvf_handler&) = delete;          // 禁止拷贝赋值
        mrvf_handler& operator=(mrvf_handler&&) = delete;               // 禁止移动赋值
    };
}

#include "core/ring/mrvf/mrvf_handler.tpp"

#endif // !MRVF_HANDLER_HPP



// // 计算文件大小(字节数)
// const size_t file_size
// = c_BOF_LEN                     // 文件头长度
// + c_VERSION_LEN                 // 版本号字段长度
// + c_CRC64_STANDARD_LEN          // CRC64计算标准字段长度
// + c_RING_SIZE_LEN               // 环大小字段长度
// + c_RVECTOR_SIZE_LEN            // 向量大小字段长度
// + m_rvector_size * sizeof(T)    // 数据载荷字节数 = 向量大小*环大小
// + c_CRC64_LEN                   // 校验字段长度
// + c_EOF_LEN;                    // 文件尾长度

// // 新建缓存区
// std::unique_ptr<uint8_t[]> file_buffer = std::make_unique<uint8_t[]>(file_size);
// uint8_t* const file_buffer_raw = file_buffer.get();

// // 初始化写指针
// size_t ipointer = 0ULL;

// // 向缓存区写入文件头
// for (size_t i = 0;i < c_BOF_LEN;++i)
// {
//     file_buffer_raw[i + ipointer] = c_BOF[i];
// }
// ipointer += c_BOF_LEN;

// // 向缓存区写入文件版本号
// file_buffer_raw[ipointer] = c_VERSION;
// ipointer += c_VERSION_LEN;

// // 向缓存区写入指定的CRC64计算标准
// file_buffer_raw[ipointer] = m_crc64_standard;
// ipointer += c_CRC64_STANDARD_LEN;

// // 向缓存区写入环大小
// file_buffer_raw[ipointer] = m_ring_size;
// ipointer += c_RING_SIZE_LEN;

// // 写入向量大小（数据段长度）
// std::memcpy
// (
//     reinterpret_cast<char*>(file_buffer_raw) + ipointer,
//     reinterpret_cast<char*>(&m_rvector_size),
//     c_RVECTOR_SIZE_LEN
// );
// ipointer += c_RVECTOR_SIZE_LEN;

// // 写入向量数据
// std::memcpy
// (
//     reinterpret_cast<char*>(file_buffer_raw) + ipointer,
//     reinterpret_cast<char*>(m_rvector_segment.get()),
//     m_rvector_size * sizeof(RT)
// );
// ipointer += (m_rvector_size * sizeof(RT));

// // 实例化crc计算接口
// std::unique_ptr<crc64> crc64_interface = crc64_factory::acquire_interface
// (
//     c_UINT8_T_TO_CRC64_STANDARD(m_crc64_standard)
// );

// // 计算CRC64校验码
// const size_t crc64_compute_len
// = c_VERSION_LEN
// + c_CRC64_STANDARD_LEN
// + c_RING_SIZE_LEN
// + c_RVECTOR_SIZE_LEN
// + m_rvector_size * sizeof(RT);

// uint64_t crc64_checksum = crc64_interface->compute
// (
//     file_buffer_raw + c_BOF_LEN,
//     crc64_compute_len
// );

// // 写入CRC64校验码
// std::memcpy
// (
//     reinterpret_cast<char*>(file_buffer_raw) + ipointer,
//     reinterpret_cast<char*>(&crc64_checksum),
//     c_CRC64_LEN
// );
// ipointer += c_CRC64_LEN;

// // 向缓存区写入文件尾
// for (size_t i = 0;i < c_EOF_LEN;++i)
// {
//     file_buffer_raw[i + ipointer] = c_EOF[i];
// }
// ipointer += c_EOF_LEN;

// // 一般断言是没问题的
// MPMT_ASSERT(ipointer == file_size, "File write operation was unexpectedly interrupted.");

// // 写入文件
// out_file.write
// (
//     reinterpret_cast<const char*>(file_buffer_raw),
//     file_size
// );