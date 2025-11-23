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

        mpmt::rvector<RT>               m_rvector;           // 向量
        const crc64_factory::standard   mc_crc64_standard;   // crc64计算标准
        const uint8_t                   mc_ring_size;        // 环大小
    };

    template<typename RT>
    class mrvf_handler
    {
    public:
        struct config
        {
            bool m_use_memory_map;                  // 读写文件时是否启用内存映射
            bool m_validate_checksum;               // 读取文件时是否验证校验码
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
        const mrvf_handler::config mc_config;                           // 加载、保存配置设置

        /** @brief mrcf file format bit size (Unit: Bytes) */
        static constexpr size_t mc_BOF_BYTE_SIZE             = 8;       // 文件头长度
        static constexpr size_t mc_CRC64_STANDARD_BYTE_SIZE  = 1;       // CRC64计算标准字段长度
        static constexpr size_t mc_RING_SIZE_BYTE_SIZE       = 1;       // 环大小字段长度
        static constexpr size_t mc_RVECTOR_SIZE_BYTE_SIZE    = 4;       // 向量大小字段长度
        static constexpr size_t mc_CRC64_BYTE_SIZE           = 8;       // CRC64字段长度
        static constexpr size_t mc_EOF_BYTE_SIZE             = 8;       // 文件尾长度

        /** @brief constant value */
        static constexpr uint8_t mc_BOF[mc_BOF_BYTE_SIZE] =             // 文件头-标识"MRVF_BOF"
        {
            0x4d,0x52,0x56,0x46,0x5f,0x42,0x4f,0x46
        };
        static constexpr uint8_t mc_EOF[mc_EOF_BYTE_SIZE] =             // 文件尾-标识"MRVF_EOF"
        {
            0x4d,0x52,0x56,0x46,0x5f,0x45,0x4f,0x46
        };

        static const inline std::string mc_FILE_EXTENSION = ".mrvf";    // 文件拓展名

        static const inline std::unordered_map <                        // enum到字段值的健壮转换
            crc64_factory::standard,
            uint8_t
        > mc_CRC64_STANDARD_TO_UINT8_T =
        {
            { crc64_factory::standard::ECMA182 , 0x00 },
            { crc64_factory::standard::ISO     , 0x01 }
        };

        static const inline std::unordered_map <                        // 字段值到enum的健壮转换
            uint8_t,
            crc64_factory::standard
        > mc_UINT8_T_TO_CRC64_STANDARD =
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