#ifndef RVECTOR_HPP
#define RVECTOR_HPP

#include <memory>
#include <vector>

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
            is_ring_type,
            "RT must be ring1, ring8, ring16, ring32, or ring64."
            );

        rvector();                                      // 默认构造
        explicit rvector(size_t n);                     // 指定大小构造
        rvector(size_t n, const RT value);              // 指定大小 + 默认值构造
        rvector(const std::vector<RT>& list);           // 列表构造
        rvector(const rvector& other);                  // 拷贝构造
        rvector(rvector&& other) noexcept;              // 移动构造

        /**
         * @brief   拷贝赋值
         * @param   const rvector<RT>& other 拷贝对象
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT>& operator=(const rvector<RT>& other);

        /**
         * @brief   移动赋值
         * @param   rvector<RT>&& other 移动对象
         * @return  rvector<RT>& 当前向量的引用
         */
        rvector<RT>& operator=(rvector<RT>&& other) noexcept;

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

        /**
         * @brief   向量重载等于比较操作符
         * @param   const rvector<RT>& other
         * @return  bool 比较结果
         */
        bool operator==(const rvector<RT>& other) const;

        /**
         * @brief   向量重载不等比较操作符
         * @param   rvector<RT>& other
         * @return  bool 比较结果
         */
        bool operator!=(const rvector<RT>& other) const;

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

        friend void rvector_load();
        friend void rvector_load();

    private:
        std::unique_ptr<RT[]> m_data_;
        size_t m_size_;

        /** @brief 禁用大于运算符 */
        bool operator>(const rvector<RT>& other) const = delete;

        /** @brief 禁用小于运算符 */
        bool operator<(const rvector<RT>& other) const = delete;

        /** @brief 禁用大于等于运算符 */
        bool operator>=(const rvector<RT>& other) const = delete;

        /** @brief 禁用小于等于运算符 */
        bool operator<=(const rvector<RT>& other) const = delete;
    };
}

extern template class mpmt::rvector<mpmt::ring1>;
extern template class mpmt::rvector<mpmt::ring8>;
extern template class mpmt::rvector<mpmt::ring16>;
extern template class mpmt::rvector<mpmt::ring32>;
extern template class mpmt::rvector<mpmt::ring64>;

#endif // !RVECTOR_HPP
