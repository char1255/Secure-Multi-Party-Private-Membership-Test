#ifndef RANDOM_ADAPTER_HPP
#define RANDOM_ADAPTER_HPP

#include <cstdint>
#include <memory>
#include <type_traits>
#include <algorithm>


//////////////////////////////////////////////////////////////////////////////////////
// 重要问题：RAND_bytes(...)最大长度仅支持32，不支持64，需要修改为分块版本以支持uint64_ t分割
///////////////////////////////////////////////////////////////////////////////////////


/** @namespace 项目命名空间。 */
namespace mpmt
{

    template<typename DT>
    constexpr bool is_rng_supported =
        std::is_same_v<DT, uint64_t> 
        || std::is_same_v<DT, uint32_t> 
        || std::is_same_v<DT, uint16_t> 
        || std::is_same_v<DT, uint8_t>;

    template <typename DT>
    struct rng_array
    {
        /** @brief 断言限制模板类型 */
        static_assert(
            is_rng_supported<DT>,
            "DT must be uint8_t, uint16_t, uint32_t or uint64_t."
        );

        explicit rng_array(const uint64_t size)
            :
            m_data(size == 0 ? nullptr : std::make_unique<DT[]>(size)),
            m_size(size)
        {}

        explicit rng_array(const uint64_t size, const DT default_value)
            : rng_array(size)
        {
            if (m_size != 0) {
                std::fill(m_data.get(), m_data.get() + m_size, default_value);
            }
        }

        rng_array() = delete;
        rng_array(const rng_array&) = delete;
        rng_array& operator=(const rng_array&) = delete;

        rng_array(rng_array&&) noexcept = default;
        rng_array& operator=(rng_array&&) noexcept = default;

        std::unique_ptr<DT[]> m_data;
        uint64_t m_size;
    };

    /**
     * @class   随机数适配器，用于封装不同实现的随机数接口。
     * @tparam  DT 随机数数据类型，限定为 uint8, uint16 uint32, uint64
     */
    template <typename DT>
    class rng_adapter
    {
    public:
        /** @brief 断言限制模板类型 */
        static_assert(
            is_rng_supported<DT>,
            "DT must be uint8_t, uint16_t, uint32_t or uint64_t."
        );

        /**
         * @brief   返回\mathbb{Z}_{2^n}上的随机数。
         * @return  DT 随机数。
         */
        virtual DT rand() const = 0;

        /**
         * @brief   返回\mathbb{Z}_{2^n}上的随机数数组。
         * @param   const uint64_t size 随机数数组的大小
         * @return  rng_array<DT> 随机数数组。
         */
        virtual rng_array<DT> rand(const uint64_t size) const = 0;


        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^n}上的随机数。
         * @param   const DT lb 取值下界。
         * @param   const DT ub 取值上界。
         * @return  DT 随机数。
         * @note    需要保障 lb <= ub。
         */
        virtual DT rand(const DT lb, const DT ub) const = 0;

        /**
         * @brief   返回属于[lb, ub]\mathbb{Z}_{2^n}上，大小为size的随机数数组
         * @param   const DT lb 取值下界
         * @param   const DT ub 取值上界
         * @param   const uint64_t size 随机数数组的大小
         * @return  rng_array<DT>
         * @note    需要保障 lb <= ub。
         */
        virtual rng_array<DT> rand
        (
            const DT lb, 
            const DT ub, 
            const uint64_t size
        ) const = 0;

        /**
         * @brief   析构接口。
         */
        virtual ~rng_adapter() = default;
    };

} // namespace mpmt
#endif // !RANDOM_ADAPTER_HPP