#ifndef RANDOM_ADAPTER_HPP
#define RANDOM_ADAPTER_HPP

#include <type_traits>
#include <vector>

/** @namespace 项目命名空间。 */
namespace mpmt
{

    /**
     * @class   随机数适配器，用于封装不同实现的随机数接口。
     * @tparam  DT 随机数数据类型，限定为 ring8, ring16 ring32, ring64
     */
    template <typename DT>
    class rng_adapter
    {
    public:
        /** @brief 断言限制模板类型 */
        static_assert(
            std::is_same_v<DT, uint64_t> 
            || std::is_same_v<DT, uint32_t> 
            || std::is_same_v<DT, uint16_t> 
            || std::is_same_v<DT, uint8_t>,
            "DT must be ring1, ring8, ring16, ring32, ring64."
        );

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数。
         * @return  DT 随机数。
         */
        virtual DT rand() const = 0;

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数数组。
         * @param   const uint64_t num 随机数数组的大小
         * @return  std::vector<DT> 随机数数组。
         * @note    1. 需要保障 lb <= ub。
         *          2. 尽量使用移动语义处理返回。
         */
        virtual std::vector<DT> rand(const uint64_t num) const = 0;

        /**
         * @brief   返回\mathbb{Z}_{2^32}上的随机数数组。
         * @param   const uint64_t num 随机数数组的大小
         * @return  void
         * @note    1. 需要保障 lb <= ub。
         *          2. 如何rands大小与num一致。
         */
        virtual void rand(const uint64_t num, std::vector<DT> &rands) const = 0;

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
         * @param   const uint64_t num 随机数数组的大小
         * @return  std::vector<DT> 随机数数组
         * @note    1. 需要保障 lb <= ub。
         *          2. 尽量使用移动语义处理返回。
         */
        virtual std::vector<DT> rand(const DT lb, const DT ub, const uint64_t num) const = 0;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^32}上，大小为num的随机数数组
         * @param   const DT lb 取值下界
         * @param   const DT ub 取值上界
         * @param   const uint64_t num 随机数数组的大小
         * @param   std::vector<DT>& rands 用于返回生成的随机数数组
         * @return  void
         * @note    1. 需要保障 lb <= ub。
         *          2. 如何rands大小与num一致。
         */
        virtual void rand
        (
            const DT lb, 
            const DT ub, 
            const uint64_t num,
            std::vector<DT> &rands
        ) const = 0;

        /**
         * @brief   析构接口。
         */
        virtual ~rng_adapter() = default;
    };

} // namespace mpmt
#endif // !RANDOM_ADAPTER_HPP