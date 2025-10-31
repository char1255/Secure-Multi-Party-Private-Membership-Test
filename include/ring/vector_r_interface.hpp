#ifndef VECTOR_R_HPP
#define VECTOR_R_HPP

#include <type_traits>

#include "mpmtcfg.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{

    /**
     * @class   环上数组统一接口
     * @tparam  RT 环类型，限定为ring1,ring32，ring64
     */
    template <typename RT>
    class vector_r_interface
    {
        /** @brief 断言限制模板类型 */
        static_assert(
            std::is_same<RT, ring1>::value || std::is_same<RT, ring32>::value || std::is_same<RT, ring64>::value,
            "RT must be ring1, ring32, or ring64"
        );

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 暴露迭代器接口

        /** @class 前向声明迭代器类型，由实现类提供 */
        class iterator;

        /** @class 前向声明常量迭代器类型，由实现类提供 */
        class const_iterator;

        /**
         * @brief   获取起始迭代器
         * @return  iterator 指向第一个元素的迭代器
         */
        virtual iterator begin() = 0;

        /**
         * @brief 获取结束迭代器
         * @return iterator 指向最后一个元素之后位置的迭代器
         */
        virtual iterator end() = 0;

        /**
         * @brief 获取常量起始迭代器
         * @return const_iterator 指向第一个元素的常量迭代器
         */
        virtual const_iterator begin() const = 0;

        /**
         * @brief 获取常量结束迭代器
         * @return const_iterator 指向最后一个元素之后位置的常量迭代器
         */
        virtual const_iterator end() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 重载向量操作符

        /**
         * @brief   下标访问运算符
         * @param   size_t index 索引位置
         * @return  RT& 对应位置的元素引用
         */
        virtual RT &operator[](size_t index) = 0;

        /**
         * @brief   常量下标访问运算符
         * @param   size_t index 索引位置
         * @return  const RT& 对应位置的常量元素引用
         */
        virtual const RT &operator[](size_t index) const = 0;

        /**
         * @brief   向量-向量加法
         * @param   const vector_r_interface<RT>& other 要相加的向量
         * @return  vector_r_interface<RT>& 当前向量的引用
         */
        virtual vector_r_interface<RT> &operator+=(const vector_r_interface<RT> &other) = 0;

        /**
         * @brief   向量-标量加法
         * @param   const RT scalar 要相乘的标量
         * @return  vector_r_interface<RT>& 当前向量的引用
         */
        virtual vector_r_interface<RT> &operator+=(const RT scalar) = 0;

        /**
         * @brief   向量-向量减法
         * @param   const vector_r_interface<RT>& other 要相减的向量
         * @return  vector_r_interface<RT>& 当前向量的引用
         */
        virtual vector_r_interface<RT> &operator-=(const vector_r_interface<RT> &other) = 0;

        /**
         * @brief   向量-标量减法
         * @param   const RT scalar 要相乘的标量
         * @return  vector_r_interface<RT>& 当前向量的引用
         */
        virtual vector_r_interface<RT> &operator-=(const RT scalar) = 0;

        /**
         * @brief   向量哈达玛积
         * @param   const vector_r_interface<RT>& other 要相乘的向量
         * @return  vector_r_interface<RT>& 当前向量的引用
         */
        virtual vector_r_interface<RT> &operator*=(const vector_r_interface<RT> &other) = 0;

        /**
         * @brief   向量-标量乘法法
         * @param   const RT scalar 要相乘的标量
         * @return  vector_r_interface<RT>& 当前向量的引用
         */
        virtual vector_r_interface<RT> &operator*=(const RT scalar) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 其他

        /**
         * @brief   常量下标访问运算符
         * @return  size_t 向量大小
         */
        virtual size_t size() const noexcept = 0;

        /**
         * @brief   析构接口
         * @param   void
         * @return  void
         */
        virtual ~vector_r_interface() = 0;
    };
}
#endif // !VECTOR_R_HPP
