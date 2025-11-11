#include "core/ring/rvector.hpp"
#include "core/exception/std_exc.hpp"

template<typename RT>
mpmt::rvector<RT>::rvector() :
    m_data_(nullptr),
    m_size_(0)
{}

template<typename RT>
mpmt::rvector<RT>::rvector(size_t n) :
    m_size_(n),
    m_data_(std::make_unique<RT[]>(n))
{}

template<typename RT>
mpmt::rvector<RT>::rvector
(
    size_t n,
    const RT value
) : rvector(n)
{
    std::fill(m_data_.get(), m_data_.get() + m_size_, value);
}

template<typename RT>
mpmt::rvector<RT>::rvector(std::initializer_list<RT> init_list)
    : rvector(init_list.size())
{
    std::copy(init_list.begin(), init_list.end(), m_data_.get());
}

template <typename RT>
mpmt::rvector<RT>::rvector(const rvector& other)
    :
    rvector(other.m_size_)
{
    std::copy(other.m_data_.get(), other.m_data_.get() + m_size_, m_data_.get());
}

template<typename RT>
mpmt::rvector<RT>::rvector(rvector&& other) noexcept
    : m_data_(std::move(other.m_data_)), m_size_(other.m_size_)
{
    other.m_data_ = nullptr;
    other.m_size_ = 0;
}

template <typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator=(const rvector<RT>& other)
{
    if (this != &other)
    {
        if (m_size_ != other.m_size_)
        {
            rvector<RT> temp(other);
            std::swap(this->m_data_, temp.m_data_);
            std::swap(this->m_size_, temp.m_size_);
        }
        else
        {
            std::copy(other.m_data_.get(), other.m_data_.get() + m_size_, m_data_.get());
        }
    }
    return *this;
}

template<typename RT>
RT& mpmt::rvector<RT>::operator[](size_t index)
{
    MPMT_ASSERT(index < m_size_, "Vector dimension mismatch for multiplication.");
    return m_data_[index];
}

template<typename RT>
const RT& mpmt::rvector<RT>::operator[](size_t index) const
{
    MPMT_ASSERT(index < m_size_, "Vector dimension mismatch for multiplication.");
    return m_data_[index];
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator+=(const rvector<RT>& other)
{
    MPMT_ASSERT(m_size_ == other.m_size_, "Vector dimension mismatch for addition.");

    for (size_t i = 0;i < m_size_;++i)
    {
        m_data_[i] += other[i];
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator+=(const RT scalar)
{
    for (size_t i = 0;i < m_size_;++i)
    {
        m_data_[i] += scalar;
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator-=(const rvector<RT>& other)
{
    MPMT_ASSERT(m_size_ == other.m_size_, "Vector dimension mismatch for subtraction.");
    for (size_t i = 0;i < m_size_;++i)
    {
        m_data_[i] -= other[i];
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator-=(const RT scalar)
{
    for (size_t i = 0;i < m_size_;++i)
    {
        m_data_[i] -= scalar;
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator*=(const rvector<RT>& other)
{
    MPMT_ASSERT(m_size_ == other.m_size_, "Vector dimension mismatch for multiplication.");
    for (size_t i = 0;i < m_size_;++i)
    {
        m_data_[i] *= other[i];
    }

    return *this;
}

template<typename RT>
mpmt::rvector<RT>& mpmt::rvector<RT>::operator*=(const RT scalar)
{
    for (size_t i = 0;i < m_size_;++i)
    {
        m_data_[i] *= scalar;
    }

    return *this;
}

template<typename RT>
bool mpmt::rvector<RT>::operator==(const rvector<RT>& other) const
{
    if (this->m_size_ != other.m_size_)
    {
        return false;
    }

    for (size_t i = 0;i < m_size_;++i)
    {
        if (m_data_[i] != other.m_data_[i])
        {
            return false;
        }
    }
    return true;
}

template<typename RT>
bool mpmt::rvector<RT>::operator!=(const rvector<RT>& other) const
{
    return !(*this == other);
}

template<typename RT>
RT mpmt::rvector<RT>::reduce() const noexcept
{
    RT reduction = 0;
    for (size_t i = 0; i < m_size_; ++i)
    {
        reduction += m_data_[i];
    }
    return reduction;
}

template<typename RT>
size_t mpmt::rvector<RT>::size() const noexcept
{
    return m_size_;
}

template<typename RT>
mpmt::rvector<RT>::~rvector() { /** 智能指针自动析构 */ }

////////////////////////////////////////////////////////////////////////////////////////////////////
// 显式实例化

template class mpmt::rvector<mpmt::ring1>;
template class mpmt::rvector<mpmt::ring8>;
template class mpmt::rvector<mpmt::ring16>;
template class mpmt::rvector<mpmt::ring32>;
template class mpmt::rvector<mpmt::ring64>;