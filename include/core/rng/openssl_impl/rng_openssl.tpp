#ifndef RANDOM_OPENSSL_TPP
#define RANDOM_OPENSSL_TPP

#include "core/rng/openssl_impl/rng_openssl.hpp"
#include "core/exception/rng_exc.hpp"
template <typename DT>
DT mpmt::random_openssl<DT>::rand() const
{
   if constexpr (std::is_same_v<DT, ring1>)
   {
      ring8 r;
      if (RAND_bytes((unsigned char *)&r, sizeof(r)) != 1)
      {
         throw mpmt::rng_exc("Random byte generation failed: low entropy or internal error.")
      }
      return ring1(r);
   }
   else
   {
      DT r;
      if (RAND_bytes((unsigned char *)&r, sizeof(r)) != 1){
         throw mpmt::rng_exc("Random byte generation failed: low entropy or internal error.");
      }
      return r;
   }
}

template <typename DT>
DT mpmt::random_openssl<DT>::rand(const DT lb, const DT ub) const
{
   static_assert(!std::is_same_v<DT, ring1>, "mpmt::random_openssl<DT>::rand(const DT lb, const DT ub) does not support ring1.");
   
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