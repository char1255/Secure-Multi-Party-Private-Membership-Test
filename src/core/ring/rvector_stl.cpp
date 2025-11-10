#include "core/ring/rvector.hpp"
#include "core/exception/std_exc.hpp"

template<typename RT>
mpmt::rvector<RT>::rvector() :
    m_data(nullptr),
    m_size(0)
{}

template<typename RT>
mpmt::rvector<RT>::rvector(size_t n) :
    m_size(n),
    m_data(new(std::nothrow) RT[m_size])
{
    if (!m_data)
    {
        throw mpmt::std_exc("Memory allocation failed.");
    }
}

template<typename RT>
mpmt::rvector<RT>::rvector
(
    size_t n,
    const RT value
)
    : rvector(n)
{
    std::fill(m_data, m_data + m_size, value);
}

template<typename RT>
mpmt::rvector<RT>::rvector(std::initializer_list<RT> init_list)
    : rvector(init_list.size())
{
    std::copy(init_list.begin(), init_list.end(), m_data);
}

// 迭代器接口
// ...
// 迭代器接口

template<typename RT>
RT& mpmt::rvector<RT>::operator[](size_t index)
{
    return m_data[index];
}

template<typename RT>
const RT& mpmt::rvector<RT>::operator[](size_t index) const
{
    return m_data[index];
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator+=(const rvector<RT>& other)
{
    MPMT_ASSERT(m_size == other.m_size, "向量加法维度并未对齐");
    for (size_t i = 0;i < m_size;++i)
    {
        m_data[i] += other[i];
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator+=(const RT scalar)
{
    for (size_t i = 0;i < m_size;++i)
    {
        m_data[i] += scalar;
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator-=(const rvector<RT>& other)
{
    MPMT_ASSERT(m_size == other.m_size, "向量减法维度并未对齐");
    for (size_t i = 0;i < m_size;++i)
    {
        m_data[i] -= other[i];
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator-=(const RT scalar)
{
    for (size_t i = 0;i < m_size;++i)
    {
        m_data[i] -= scalar;
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator*=(const rvector<RT>& other)
{
    MPMT_ASSERT(m_size == other.m_size, "向量乘法维度并未对齐");
    for (size_t i = 0;i < m_size;++i)
    {
        m_data[i] *= other[i];
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator*=(const RT scalar)
{
    for (size_t i = 0;i < m_size;++i)
    {
        m_data[i] *= scalar;
    }

    return *this;
}

template<typename RT>
RT mpmt::rvector<RT>::reduce() const noexcept
{
    RT reduction = RT{ 0 };
    for (size_t i = 0; i < m_size; ++i)
    {
        reduction += m_data[i];
    }
    return reduction;
}

template<typename RT>
size_t mpmt::rvector<RT>::size() const noexcept
{
    return m_size;
}

template<typename RT>
mpmt::rvector<RT>::~rvector()
{
    delete[] m_data;
}


template class mpmt::rvector<mpmt::ring1>;
template class mpmt::rvector<mpmt::ring8>;
template class mpmt::rvector<mpmt::ring16>;
template class mpmt::rvector<mpmt::ring32>;
template class mpmt::rvector<mpmt::ring64>;