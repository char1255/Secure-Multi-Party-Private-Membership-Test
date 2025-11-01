#ifndef VECTOR_R64_HPP
#define VECTOR_R64_HPP

#include "mpmtcfg.hpp"
#include "core/ring/vector_r_interface.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    /**
     * @class   环\mathbb{Z}_{2^64}上的数组
     */
    class vector_r64 : public vector_r_interface<ring64>
    {
    public:
        iterator begin() override;
        iterator end() override;
        const_iterator begin() const override;
        const_iterator end() const override;
        ring64 &operator[](size_t index) override;
        const ring64 &operator[](size_t index) const override;
        virtual vector_r64& operator+=(const vector_r64& other) = 0;
        virtual vector_r64& operator+=(const ring64 scalar) = 0;
        virtual vector_r64& operator-=(const vector_r64& other) = 0;
        virtual vector_r64& operator-=(const ring64 scalar) = 0;
        virtual vector_r64& operator*=(const vector_r64& other) = 0;
        virtual vector_r64& operator*=(const ring64 scalar) = 0;
        size_t size() const noexcept override;
        ~vector_r64() = default;
    };
}

#endif // !VECTOR_R64_HPP