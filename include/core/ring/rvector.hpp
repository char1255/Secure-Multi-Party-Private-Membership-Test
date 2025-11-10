#ifndef RVECTOR_HPP
#define RVECTOR_HPP

#include <type_traits>

#include "core/mpmtcfg.hpp"
#include "core/ring/ring.hpp"

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
    public:

        /** @brief 断言限制模板类型 */
        static_assert(
            std::is_same_v<RT, ring1> ||
            std::is_same_v<RT, ring8> ||
            std::is_same_v<RT, ring16> ||
            std::is_same_v<RT, ring32> ||
            std::is_same_v<RT, ring64>,
            "RT must be ring1, ring8, ring16, ring32, or ring64."
            );

        rvector();                                      // 默认构造

        explicit rvector(size_t n);                     // 指定大小构造

        rvector(size_t n, const RT value);              // 指定大小 + 默认值构造

        rvector(std::initializer_list<RT> init_list);   // 初始化列表构造

        rvector(const rvector& other);                  // 拷贝构造

        rvector(rvector&& other) noexcept;              // 移动构造

        template<typename InputIt>                      // 范围构造
        rvector(InputIt first, InputIt last);

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
        iterator begin();

        /**
         * @brief 获取结束迭代器
         * @return iterator 指向最后一个元素之后位置的迭代器
         */
        iterator end();

        /**
         * @brief 获取常量起始迭代器
         * @return const_iterator 指向第一个元素的常量迭代器
         */
        const_iterator begin() const;

        /**
         * @brief 获取常量结束迭代器
         * @return const_iterator 指向最后一个元素之后位置的常量迭代器
         */
        const_iterator end() const;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 重载向量操作符

        /**
         * @brief   下标访问运算符
         * @param   size_t index 索引位置
         * @return  RT& 对应位置的元素引用
         */
        RT& operator[](size_t index);

        /**
         * @brief   常量下标访问运算符
         * @param   size_t index 索引位置
         * @return  const RT& 对应位置的常量元素引用
         */
        const RT& operator[](size_t index) const;

        /**
         * @brief   向量-向量加法
         * @param   const rvector<RT>& other 要相加的向量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT>& operator+=(const rvector<RT>& other);

        /**
         * @brief   向量-标量加法
         * @param   const RT scalar 要相乘的标量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT>& operator+=(const RT scalar);

        /**
         * @brief   向量-向量减法
         * @param   const rvector<RT>& other 要相减的向量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT>& operator-=(const rvector<RT>& other);

        /**
         * @brief   向量-标量减法
         * @param   const RT scalar 要相乘的标量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT>& operator-=(const RT scalar);

        /**
         * @brief   向量哈达玛积
         * @param   const rvector<RT>& other 要相乘的向量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT>& operator*=(const rvector<RT>& other);

        /**
         * @brief   向量-标量乘法法
         * @param   const RT scalar 要相乘的标量
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT>& operator*=(const RT scalar);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 其他

        /**
         * @brief   获取向量和
         * @return  RT 环上向量和
         */
        RT reduce() const noexcept;

        /**
         * @brief   常量下标访问运算符
         * @return  size_t 向量大小
         */
        size_t size() const noexcept;

        /**
         * @brief   析构接口
         * @param   void
         * @return  void
         */
        ~rvector();

    private:
        RT* m_data;
        size_t m_size;
    };

    /** @brief ring1类型完全特化 */
    template <>
    class rvector<ring1>
    {
    public:
    };

}

extern template class mpmt::rvector<mpmt::ring1>;
extern template class mpmt::rvector<mpmt::ring8>;
extern template class mpmt::rvector<mpmt::ring16>;
extern template class mpmt::rvector<mpmt::ring32>;
extern template class mpmt::rvector<mpmt::ring64>;

#endif // !RVECTOR_HPP
