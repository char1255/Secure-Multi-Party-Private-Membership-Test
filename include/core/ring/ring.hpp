#ifndef RING_HPP
#define RING_HPP
#include <cstdint>
#include <stdexcept>
#include <iostream>
/** @namespace 项目命名空间。*/
namespace mpmt
{
    // 实验时全部采用\mathbb{Z}_{2^n}：模2^n的整数环

    /**
     * @class       ring1
     * @brief       1位环
     * @internal    内部数据类型，without注释
     * @note        仅仅在临时从rvector获取值时使用，实际大小是8bit。
     */
    class ring1
    {
    public:
        ring1() noexcept : m_v(0) {}
        ring1(uint8_t v) noexcept : m_v(v & 1) {}
        ring1(const ring1& o) noexcept = default;
        ring1& operator=(uint8_t v) noexcept { m_v = v & 1; return *this; }
        ring1& operator=(const ring1& v) noexcept { m_v = v.m_v; return *this; }
        ring1 operator+(const ring1& other) const noexcept { return ring1(m_v ^ other.m_v); }
        ring1 operator-(const ring1& other) const noexcept { return ring1(m_v ^ other.m_v); }
        ring1 operator*(const ring1& other) const noexcept { return ring1(m_v & other.m_v); }
        ring1& operator+=(const ring1& other) noexcept { m_v ^= other.m_v; return *this; }
        ring1& operator-=(const ring1& other) noexcept { m_v ^= other.m_v; return *this; }
        ring1& operator*=(const ring1& other) noexcept { m_v &= other.m_v; return *this; }
        bool operator==(const ring1& other) const noexcept { return m_v == other.m_v; }
        bool operator!=(const ring1& other) const noexcept { return m_v != other.m_v; }
        friend std::ostream& operator<<(std::ostream& os, const ring1& r) { return os << static_cast<int>(r.m_v); }

        template<typename RT>
        RT fill_bits();

    private:
        uint8_t m_v;
        ring1 operator^(const ring1&) = delete;
        ring1& operator^=(const ring1&) = delete;
        ring1 operator|(const ring1&) = delete;
        ring1& operator|=(const ring1&) = delete;
        ring1 operator/(const ring1&) = delete;
        ring1& operator/=(const ring1&) = delete;
        bool operator>(const ring1&) = delete;
        bool operator<(const ring1&) = delete;
        bool operator>=(const ring1&) = delete;
        bool operator<=(const ring1&) = delete;
    };

    /** @typedef  */
    using ring8 = uint8_t;

    /** @typedef 16位环 */
    using ring16 = uint16_t;

    /** @typedef 32位环。 */
    using ring32 = uint32_t;

    /** @typedef 64位环。 */
    using ring64 = uint64_t;

    template <typename RT>
    inline RT boolean_to_arithmetic(const ring1 x)
    {
        static_assert(
            is_ring_type && !std::is_same_v<RT, ring1>,
            "RT must be ring8, ring16, ring32, or ring64."
            );

        return RT(); // TO BE UPDATED
    }

    template <typename RT>
    inline ring1 arithmetic_to_boolean(const RT x)
    {
        static_assert(
            is_ring_type && !std::is_same_v<RT, ring1>,
            "RT must be ring8, ring16, ring32, or ring64."
            );

        return ring1(); // TO BE UPDATED
    }

    template<typename RT>
    RT ring1::fill_bits()
    {
        static_assert(
            is_ring_type && !std::is_same_v<RT, ring1>,
            "RT must be ring8, ring16, ring32, or ring64."
            );

        RT data = static_cast<RT>(0);
        return (m_v == 0) ? data : ~data;
    }

    template<typename RT>
    constexpr bool is_ring_type =
        std::is_same_v<RT, ring1>   ||
        std::is_same_v<RT, ring8>   ||
        std::is_same_v<RT, ring16>  ||
        std::is_same_v<RT, ring32>  ||
        std::is_same_v<RT, ring64>;

}
#endif // !RING_HPP