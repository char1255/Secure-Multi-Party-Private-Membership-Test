#ifndef RING_HPP
#define RING_HPP
#include <cstdint>
#include <stdexcept>
#include <iostream>
/** @namespace 项目命名空间。*/
namespace mpmt
{
    /** 
     * @class       ring1
     * @brief       1位环
     * @internal    内部数据类型，without注释
     * @note        仅仅在临时从rvector获取值时使用，实际大小是8bit。
     */
    class ring1
    {
    public:
        ring1() noexcept : _v(0) {}
        ring1(uint8_t v) noexcept : _v(v & 1) {}
        ring1(const ring1 &o) noexcept = default;
        ring1& operator=(uint8_t v) noexcept { _v = v & 1; return *this; }
        ring1& operator=(const ring1& v) noexcept { _v = v._v; return *this; }
        ring1 operator+(const ring1& other) const noexcept { return ring1(_v ^ other._v); }
        ring1 operator-(const ring1& other) const noexcept { return ring1(_v ^ other._v); }
        ring1 operator*(const ring1& other) const noexcept { return ring1(_v & other._v); }
        ring1& operator+=(const ring1& other) noexcept { _v ^= other._v; return *this; }
        ring1& operator-=(const ring1& other) noexcept { _v ^= other._v; return *this; }
        ring1& operator*=(const ring1& other) noexcept { _v &= other._v; return *this; }
        bool operator==(const ring1& other) const noexcept { return _v == other._v; }
        bool operator!=(const ring1& other) const noexcept { return _v != other._v; }
        friend std::ostream& operator<<(std::ostream& os, const ring1& r) { return os << static_cast<int>(r._v); }

    private:
        uint8_t _v;
        ring1 operator^(const ring1 &) = delete;
        ring1& operator^=(const ring1 &) = delete;
        ring1 operator|(const ring1 &) = delete;
        ring1& operator|=(const ring1 &) = delete;
        ring1 operator/(const ring1 &) = delete;
        ring1& operator/=(const ring1 &) = delete;
        bool operator>(const ring1 &) = delete;
        bool operator<(const ring1 &) = delete;
        bool operator>=(const ring1 &) = delete;
        bool operator<=(const ring1 &) = delete;
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
    inline RT boolean_to_arithmetic();

}
#endif // !RING_HPP