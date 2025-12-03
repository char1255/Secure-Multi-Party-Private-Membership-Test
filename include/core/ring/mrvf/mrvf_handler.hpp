#ifndef MRVF_HANDLER_HPP
#define MRVF_HANDLER_HPP

#include <string>
#include "core/mpmtcfg.hpp"
#include "core/ring/mrvf/mrvf.hpp"
#include "core/ring/ring.hpp"
#include "core/ring/rvector.hpp"
#include "core/exception/mrvf_exc.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    template<typename RT>
    class mrvf_handler
    {
    public:
        struct config
        {
            bool m_use_memory_map;          // 读写文件时是否启用内存映射
            bool m_enable_parallel_read;    // 是否启用多线程读入
        };

        /** @brief 断言限制模板类型 */
        static_assert(
            is_ring_type<RT>,
            "RT must be ring1, ring8, ring16, ring32 or ring64."
            );

        mrvf_handler(const mrvf_handler::config& config);

        /**
         * @brief 从文件中载入文件规定的环大小（用于单独判断环大小场景）
         * @param const std::string& load_path  加载路径
         * @return uint8_t 文件的环大小
         */
        uint8_t read_ring_size(const std::string& load_path);

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
        const mrvf_handler::config mc_config;                               // 加载、保存配置设置

        /** @brief mrcf file format bit size (Unit: Bytes) */
        static constexpr size_t mc_BOF_BYTE_SIZE            = 8ULL;         // 文件头长度
        static constexpr size_t mc_RING_SIZE_BYTE_SIZE      = 1ULL;         // 环大小字段长度
        static constexpr size_t mc_RVECTOR_SIZE_BYTE_SIZE   = 8ULL;         // 向量大小字段长度
        static constexpr size_t mc_CRC64_BYTE_SIZE          = 8ULL;         // CRC64字段长度
        static constexpr size_t mc_EOF_BYTE_SIZE            = 8ULL;         // 文件尾长度
        static constexpr size_t mc_MIN_FILE_SIZE =                          // 文件最小长度
            mc_BOF_BYTE_SIZE +
            mc_RING_SIZE_BYTE_SIZE +
            mc_RVECTOR_SIZE_BYTE_SIZE +
            mc_CRC64_BYTE_SIZE +
            mc_EOF_BYTE_SIZE;
        static constexpr size_t mc_MAX_RVECTOR_SIZE         = 1ULL << 50;   // 系统允许的最大长度为 50

        /** @brief constant value */
        static constexpr uint8_t mc_BOF[mc_BOF_BYTE_SIZE] =                 // 文件头-标识"MRVF_BOF"
        {
            0x4d,0x52,0x56,0x46,0x5f,0x42,0x4f,0x46
        };
        static constexpr uint8_t mc_EOF[mc_EOF_BYTE_SIZE] =                 // 文件尾-标识"MRVF_EOF"
        {
            0x4d,0x52,0x56,0x46,0x5f,0x45,0x4f,0x46
        };

        static const inline std::string mc_FILE_EXTENSION = ".mrvf";        // 文件拓展名

        /** @brief 禁用拷贝与移动操作 */
        mrvf_handler() = delete;                                            // 禁止默认构造
        mrvf_handler(const mrvf_handler&) = delete;                         // 禁止拷贝构造
        mrvf_handler(mrvf_handler&&) = delete;                              // 禁止移动构造
        mrvf_handler& operator=(const mrvf_handler&) = delete;              // 禁止拷贝赋值
        mrvf_handler& operator=(mrvf_handler&&) = delete;                   // 禁止移动赋值
    };
}

#include "core/ring/mrvf/mrvf_handler.tpp"

#endif // !MRVF_HANDLER_HPP