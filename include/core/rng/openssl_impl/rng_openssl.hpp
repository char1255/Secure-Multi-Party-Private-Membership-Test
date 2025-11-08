#ifndef RANDOM_OPENSSL_HPP
#define RANDOM_OPENSSL_HPP

#include <type_traits>
#include <vector>
#include <openssl/rand.h>

#include "core/mpmtcfg.hpp"
#include "core/rng/rng_adapter.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    /**
     * @class   使用openssl实现随机数适配器
     * @tparam  DT 随机数数据类型，限定为 ring1, ring8, ring16 ring32, ring64, size_t
     * @throw   throw mpmt::rng_exc("", mpmt::rng_exc::impl_type) 随机数生成错误
     */
    template <typename DT>
    class rng_openssl : public rng_adapter<DT>
    {
    public:

        using rcontainer = typename _ra_cont_selecter<DT>::type;
        /** @brief 默认构造函数。*/
        rng_openssl() = default;

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数。
         * @return  DT 随机数。
         */
        DT rand() const override;

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数数组。
         * @param   const size_t num 随机数数组的大小
         * @return  rcontainer 随机数数组。
         * @note    1. 需要保障 lb <= ub。
         *          2. 尽量使用移动语义处理返回。
         */
        rcontainer rand(const size_t num) const override;

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数数组。
         * @param   const size_t num 随机数数组的大小
         * @return  void
         * @note    1. 需要保障 lb <= ub。
         *          2. 如何rands大小与num一致。
         */
        void rand(
            const size_t num,
            rcontainer& rands
        ) const override;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^32}上的随机数。
         * @param   const DT lb 取值下界。
         * @param   const DT ub 取值上界。
         * @return  DT 随机数。
         * @note    需要保障 lb <= ub。
         */
        DT rand
        (
            const DT lb,
            const DT ub
        ) const override;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^32}上，大小为num的随机数数组
         * @param   const DT lb 取值下界
         * @param   const DT ub 取值上界
         * @param   const size_t num 随机数数组的大小
         * @return  rcontainer 新构造的随机数数组
         * @note    1. 需要保障 lb <= ub。
         *          2. 尽量使用移动语义处理返回。
         */
        rcontainer rand
        (
            const DT lb,
            const DT ub,
            const size_t num
        ) const override;

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
        void rand
        (
            const DT lb,
            const DT ub,
            const size_t num,
            rcontainer& rands
        ) const override;

        /**
         * @brief   析构接口。
         */
        ~rng_openssl() override = default;
    };
}

#include "core/rng/openssl_impl/rng_openssl.tpp"

#endif // !RANDOM_OPENSSL_HPP