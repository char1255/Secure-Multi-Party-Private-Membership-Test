#ifndef RANDOM_ADAPTER_HPP
#define RANDOM_ADAPTER_HPP

#include <type_traits>
#include <vector>

#include "core/mpmtcfg.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    /**
     * @class   随机数适配器，用于封装不同实现的随机数接口。
     * @tparam  DT 随机数数据类型，限定为ring32,ring64,size_t
     */
    template<typename DT>
    class random_adapter
    {
    public:
        /** @brief 断言限制模板类型 */
        static_assert(
            std::is_same<DT, ring32>::value || std::is_same_v<DT, ring64>::value || std::is_same_v<DT, size_t>::value,
            "DT must be one of: ring32, ring64, size_t");

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 以下为随机数接口
        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数。
         * @return  DT 随机数。
         */
        virtual DT rand() const = 0;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^32}上的随机数。
         * @param   const DT lb 取值下界。
         * @param   const DT ub 取值上界。
         * @return  DT 随机数。
         * @note    需要保障 lb <= ub。
         */
        virtual DT rand
        (
            const DT lb,
            const DT ub
        ) const = 0;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^32}上，大小为num的随机数数组
         * @param   const DT lb 取值下界
         * @param   const DT ub 取值上界
         * @return  std::vector<DT> 新构造的随机数数组
         * @note    1. 需要保障 lb <= ub。
         *          2. 尽量使用移动语义处理返回。
         */
        virtual std::vector<DT> rand
        (
            const DT lb,
            const DT ub, 
            const size_t num
        ) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //

        /**
         * @brief   析构接口。
         */
        virtual ~random_adapter() = 0;
    };

}
#endif // !RANDOM_ADAPTER_HPP