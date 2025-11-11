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
    m_data(std::make_unique<RT[]>(n))
{}

template<typename RT>
mpmt::rvector<RT>::rvector
(
    size_t n,
    const RT value
) : rvector(n)
{
    std::fill(m_data.get(), m_data.get() + m_size, value);
}

template<typename RT>
mpmt::rvector<RT>::rvector(std::initializer_list<RT> init_list)
    : rvector(init_list.size())
{
    std::copy(init_list.begin(), init_list.end(), m_data.get());
}

template <typename RT>
mpmt::rvector<RT>::rvector(const rvector& other)
    :
    rvector(other.m_size)
{
    std::copy(other.m_data.get(), other.m_data.get() + m_size, m_data.get());
}

template<typename RT>
mpmt::rvector<RT>::rvector(rvector&& other) noexcept
    : m_data(std::move(other.m_data)), m_size(other.m_size)
{
    other.m_data = nullptr;
    other.m_size = 0;
}

template <typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator=(const rvector<RT>& other)
{
    if (this != &other)
    {
        if (m_size != other.m_size)
        {
            rvector<RT> temp(other);
            std::swap(this->m_data, temp.m_data);
            std::swap(this->m_size, temp.m_size);
        }
        else
        {
            std::copy(other.m_data.get(), other.m_data.get() + m_size, m_data.get());
        }
    }
    return *this;
}

template<typename RT>
RT& mpmt::rvector<RT>::operator[](size_t index)
{
    MPMT_ASSERT(index < m_size, "Vector dimension mismatch for multiplication.");
    return m_data[index];
}

template<typename RT>
const RT& mpmt::rvector<RT>::operator[](size_t index) const
{
    MPMT_ASSERT(index < m_size, "Vector dimension mismatch for multiplication.");
    return m_data[index];
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator+=(const rvector<RT>& other)
{
    MPMT_ASSERT(m_size == other.m_size, "Vector dimension mismatch for addition.");

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
    MPMT_ASSERT(m_size == other.m_size, "Vector dimension mismatch for subtraction.");
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
    MPMT_ASSERT(m_size == other.m_size, "Vector dimension mismatch for multiplication.");
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
    RT reduction = 0;
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
mpmt::rvector<RT>::~rvector() { /** 智能指针自动析构 */ }

////////////////////////////////////////////////////////////////////////////////////////////////////
// 特化ring1实现

////////////////////////////////////////////////////////////////////////////////////////////////////
// 显式实例化

template class mpmt::rvector<mpmt::ring1>;
template class mpmt::rvector<mpmt::ring8>;
template class mpmt::rvector<mpmt::ring16>;
template class mpmt::rvector<mpmt::ring32>;
template class mpmt::rvector<mpmt::ring64>;