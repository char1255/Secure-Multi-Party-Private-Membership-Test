#ifndef RANDOM_ADAPTER_HPP
#define RANDOM_ADAPTER_HPP

#include <type_traits>
#include <vector>

#include "core/mpmtcfg.hpp"
#include "core/ring/ring.hpp"
#include "core/ring/rvector.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    namespace verborgen
    {
        /** @brief 模板支持参数 */
        template <typename DT>
        inline constexpr bool rng_adapter_supported =
            std::is_same_v<DT, uint64_t> || std::is_same_v<DT, uint32_t> || std::is_same_v<DT, uint16_t> || std::is_same_v<DT, uint8_t>;

        /** @brief 根据模板类型选择容器 */
        template <typename DT>
        struct radom_vector_container
        {
            /** @brief 检测类型白名单 */
            static_assert(
                rng_adapter_supported<DT>,
                "random_vector_container requires specialization for this DT type.");

            using type = void;
        };
    } // namespace verborgen

    /**
     * @class   随机数适配器，用于封装不同实现的随机数接口。
     * @tparam  DT 随机数数据类型，限定为 ring8, ring16 ring32, ring64
     */
    template <typename DT>
    class rng_adapter
    {
    public:
        using rcontainer = typename verborgen::radom_vector_container<DT>::type;

        /** @brief 断言限制模板类型 */
        static_assert(verborgen::rng_adapter_supported<DT>,
                      "DT must be ring1, ring8, ring16, ring32, ring64 or size_t.");

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数。
         * @return  DT 随机数。
         */
        virtual DT rand() const = 0;

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数数组。
         * @param   const size_t num 随机数数组的大小
         * @return  rcontainer 随机数数组。
         * @note    1. 需要保障 lb <= ub。
         *          2. 尽量使用移动语义处理返回。
         */
        virtual rcontainer rand(const size_t num) const = 0;

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数数组。
         * @param   const size_t num 随机数数组的大小
         * @return  void
         * @note    1. 需要保障 lb <= ub。
         *          2. 如何rands大小与num一致。
         */
        virtual void rand(const size_t num, rcontainer &rands) const = 0;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^32}上的随机数。
         * @param   const DT lb 取值下界。
         * @param   const DT ub 取值上界。
         * @return  DT 随机数。
         * @note    需要保障 lb <= ub。
         */
        virtual DT rand(const DT lb, const DT ub) const = 0;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^32}上，大小为num的随机数数组
         * @param   const DT lb 取值下界
         * @param   const DT ub 取值上界
         * @param   const size_t num 随机数数组的大小
         * @return  rcontainer 随机数数组
         * @note    1. 需要保障 lb <= ub。
         *          2. 尽量使用移动语义处理返回。
         */
        virtual rcontainer rand(const DT lb, const DT ub, const size_t num) const = 0;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^32}上，大小为num的随机数数组
         * @param   const DT lb 取值下界
         * @param   const DT ub 取值上界
         * @param   const size_t num 随机数数组的大小
         * @param   rcontainer& rands 用于返回生成的随机数数组
         * @return  void
         * @note    1. 需要保障 lb <= ub。
         *          2. 如何rands大小与num一致。
         */
        virtual void rand(const DT lb, const DT ub, const size_t num,
                          rcontainer &rands) const = 0;

        /**
         * @brief   析构接口。
         */
        virtual ~rng_adapter() = default;
    };

    // 特化选择类型
    template <>
    struct verborgen::radom_vector_container<uint64_t>
    {
        using type = std::vector<uint64_t>;
    };
    template <>
    struct verborgen::radom_vector_container<uint32_t>
    {
        using type = std::vector<uint32_t>;
    };
    template <>
    struct verborgen::radom_vector_container<uint16_t>
    {
        using type = std::vector<uint16_t>;
    };
    template <>
    struct verborgen::radom_vector_container<uint8_t>
    {
        using type = std::vector<uint8_t>;
    };

} // namespace mpmt
#endif // !RANDOM_ADAPTER_HPP