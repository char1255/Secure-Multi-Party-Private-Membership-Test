#ifndef VECTOR_R32_HPP
#define VECTOR_R32_HPP

#include "mpmtcfg.hpp"
#include "core/ring/vector_r_interface.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    /**
     * @class   环\mathbb{Z}_{2^32}上的数组
     */
    class vector_r32 : public vector_r_interface<ring32>
    {
    public:
        iterator begin() override;
        iterator end() override;
        const_iterator begin() const override;
        const_iterator end() const override;
        ring32 &operator[](size_t index) override;
        const ring32 &operator[](size_t index) const override;
        virtual vector_r32& operator+=(const vector_r32& other) = 0;
        virtual vector_r32& operator+=(const ring32 scalar) = 0;
        virtual vector_r32& operator-=(const vector_r32& other) = 0;
        virtual vector_r32& operator-=(const ring32 scalar) = 0;
        virtual vector_r32& operator*=(const vector_r32& other) = 0;
        virtual vector_r32& operator*=(const ring32 scalar) = 0;
        size_t size() const noexcept override;
        ~vector_r32() = default;
    };
}

#endif // !VECTOR_R32_HPP