#ifndef RANDOM_OPENSSL_TPP
#define RANDOM_OPENSSL_TPP

#include "core/randomness/openssl_impl/random_openssl.hpp"

template <typename DT>
DT mpmt::random_openssl<DT>::rand() const
{
   if constexpr (std::is_same_v<DT, ring1>) {
   } else if constexpr (std::is_same_v<DT, ring8>) {
   } else if constexpr (std::is_same_v<DT, ring16>) {
   } else if constexpr (std::is_same_v<DT, ring32>) {
   } else if constexpr (std::is_same_v<DT, ring64>) {
   }
}

template <typename DT>
DT mpmt::random_openssl<DT>::rand(const DT lb, const DT ub) const
{s
}

template <typename DT>
std::vector<DT> mpmt::random_openssl<DT>::rand(const DT lb, const DT ub, const size_t num) const
{
}

template <typename DT>
void mpmt::random_openssl<DT>::rand(const DT lb, const DT ub, const size_t num, std::vector<DT> &rands) const
{
}
#endif // !RANDOM_OPENSSL_RPP