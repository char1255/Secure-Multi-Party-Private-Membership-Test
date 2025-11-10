#ifndef RANDOM_OPENSSL_TPP
#define RANDOM_OPENSSL_TPP

#include <vector>
#include <limits>
#include "core/rng/openssl_impl/rng_openssl.hpp"
#include "core/exception/rng_exc/rng_openssl_exc.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{

   
   template <typename DT>
   DT rng_openssl<DT>::rand() const
   {
      if constexpr (std::is_same_v<DT, ring1>)
      {
         ring8 r;

         if (RAND_bytes((unsigned char*)&r, sizeof(r)) != 1)
         {
            throw rng_openssl_exc
            (
               "Random number generation failed: low entropy or internal error.",
               rng_openssl_exc::error_code::RANDOM_ENGINE_ERROR
            );
         }

         return ring1(r);
      }
      else
      {
         DT r;
         if (RAND_bytes((unsigned char*)&r, sizeof(r)) != 1)
         {
            throw rng_openssl_exc
            (
               "Random number generation failed: low entropy or internal error.",
               rng_openssl_exc::error_code::RANDOM_ENGINE_ERROR
            );
         }
         return r;
      }
   }

   template <typename DT>
   rng_openssl<DT>::rcontainer rng_openssl<DT>::rand(const size_t num) const
   {
      rng_openssl<DT>::rcontainer result;

      return std::move(result);
   }

   template <typename DT>
   void rng_openssl<DT>::rand
   (
      const size_t num,
      rng_openssl<DT>::rcontainer& rands
   ) const
   {
      if (rands.size() != num)
      {
         // resize 大小
         // 发出警告
      }
   }

   template <typename DT>
   DT rng_openssl<DT>::rand(const DT lb, const DT ub) const
   {

      static_assert(!std::is_same_v<DT, ring1>, "rng_openssl<DT>::rand(const DT lb, const DT ub) does not support ring1.");
      if (lb > ub)
      {
         throw rng_openssl_exc
         (
            "Lower bound (LB) is greater than upper bound (UB).",
            rng_openssl_exc::error_code::INVALID_INPUT
         );
      }

      const DT maxv = std::numeric_limits<DT>::max();
      if (ub == maxv && lb == 0)
      {
         return this->rand();
      }

      const DT range = (ub - lb) + 1;
      if (range == 1)
      {
         return lb;
      }

      DT r;
      const DT threshold = maxv - (maxv % range);
      do
      {
         r = this->rand();
      } while (r >= threshold);

      return (r % range) + lb;
   }

   template <typename DT>
   rng_openssl<DT>::rcontainer rng_openssl<DT>::rand
   (
      const DT lb,
      const DT ub,
      const size_t num
   ) const
   {
      rng_openssl<DT>::rcontainer result;

      return std::move(result);  // 移动语义
   }

   template <typename DT>
   void rng_openssl<DT>::rand
   (
      const DT lb,
      const DT ub,
      const size_t num,
      rng_openssl<DT>::rcontainer& rands
   ) const
   {
      if (rands.size() != num)
      {
         // resize 大小
         // 发出警告
      }
   }

}

#endif // !RANDOM_OPENSSL_RPP