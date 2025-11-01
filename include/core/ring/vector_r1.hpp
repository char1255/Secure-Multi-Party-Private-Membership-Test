#ifndef VECTOR_R1_HPP
#define VECTOR_R1_HPP

#include "mpmtcfg.hpp"
#include "core/ring/vector_r_interface.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    /**
     * @class   环\mathbb{Z}_{2^1}上的布尔数组。
     */
    class vector_r1 : public vector_r_interface<ring1>
    {
    public:
        iterator begin() override;
        iterator end() override;
        const_iterator begin() const override;
        const_iterator end() const override;
        ring1 &operator[](size_t index) override;
        const ring1 &operator[](size_t index) const override;
        virtual vector_r1& operator+=(const vector_r1& other) = 0;
        virtual vector_r1& operator+=(const ring1 scalar) = 0;
        virtual vector_r1& operator-=(const vector_r1& other) = 0;
        virtual vector_r1& operator-=(const ring1 scalar) = 0;
        virtual vector_r1& operator*=(const vector_r1& other) = 0;
        virtual vector_r1& operator*=(const ring1 scalar) = 0;
        size_t size() const noexcept override;
        ~vector_r1() = default;
    };
}

#endif // !VECTOR_R1_HPP