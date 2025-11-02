#ifndef RVECTOR_HPP
#define RVECTOR_HPP

#include <type_traits>

#include "core/mpmtcfg.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{

    /**
     * @class   环上数组统一接口
     * @tparam  RT 环类型，限定为ring1,ring32，ring64
     */
    template <typename RT>
    class rvector
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
        iterator begin() ;

        /**
         * @brief 获取结束迭代器
         * @return iterator 指向最后一个元素之后位置的迭代器
         */
        iterator end() ;

        /**
         * @brief 获取常量起始迭代器
         * @return const_iterator 指向第一个元素的常量迭代器
         */
        const_iterator begin() const ;

        /**
         * @brief 获取常量结束迭代器
         * @return const_iterator 指向最后一个元素之后位置的常量迭代器
         */
        const_iterator end() const ;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 重载向量操作符

        /**
         * @brief   下标访问运算符
         * @param   size_t index 索引位置
         * @return  RT& 对应位置的元素引用
         */
        RT &operator[](size_t index) ;

        /**
         * @brief   常量下标访问运算符
         * @param   size_t index 索引位置
         * @return  const RT& 对应位置的常量元素引用
         */
        const RT &operator[](size_t index) const ;

        /**
         * @brief   向量-向量加法
         * @param   const rvector<RT>& other 要相加的向量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT> &operator+=(const rvector<RT> &other) ;

        /**
         * @brief   向量-标量加法
         * @param   const RT scalar 要相乘的标量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT> &operator+=(const RT scalar) ;

        /**
         * @brief   向量-向量减法
         * @param   const rvector<RT>& other 要相减的向量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT> &operator-=(const rvector<RT> &other) ;

        /**
         * @brief   向量-标量减法
         * @param   const RT scalar 要相乘的标量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT> &operator-=(const RT scalar) ;

        /**
         * @brief   向量哈达玛积
         * @param   const rvector<RT>& other 要相乘的向量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT> &operator*=(const rvector<RT> &other) ;

        /**
         * @brief   向量-标量乘法法
         * @param   const RT scalar 要相乘的标量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT> &operator*=(const RT scalar) ;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 其他

        /**
         * @brief   常量下标访问运算符
         * @return  size_t 向量大小
         */
        size_t size() const noexcept ;

        /**
         * @brief   析构接口
         * @param   void
         * @return  void
         */
        ~rvector() ;

    private:
        /** @class 前向声明向量适配器，由不同的显式实例化提供 */
        class _vector_adapter;
        
    };
}

extern template class mpmt::rvector<mpmt::ring1>;
extern template class mpmt::rvector<mpmt::ring8>;
extern template class mpmt::rvector<mpmt::ring16>;
extern template class mpmt::rvector<mpmt::ring32>;
extern template class mpmt::rvector<mpmt::ring64>;

// rvector.tpp实现通用模板定义
#include "core/ring/rvector.tpp"

#endif // !RVECTOR_HPP
