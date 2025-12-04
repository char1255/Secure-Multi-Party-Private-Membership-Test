#include <string>    
#include <limits>
#include <vector>
#include "core/mpmtcfg.hpp"
#include "core/exception/rng_exc.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
   template <typename DT>
   DT rng_openssl<DT>::rand() const
   {
		DT r;
		if (RAND_bytes((unsigned char*)&r, sizeof(DT)) != 1)
		{
			throw mpmt::rng_exc
			(
				rng_exc::impl_type::OPENSSL,
				"random number generation failed, low entropy or internal error."
			);
		}
		return r;
   }

   template <typename DT>
   std::vector<DT> rng_openssl<DT>::rand(const uint64_t num) const
   {
		if (num == 0)
			return {};
		
      	std::vector<DT> result(num);
		if (RAND_bytes((unsigned char*)result.data(), sizeof(DT) * result.size()) != 1)
		{
			throw mpmt::rng_exc
			(
				rng_exc::impl_type::OPENSSL,
				"random number generation failed, low entropy or internal error."
			);
		}
		return result;
   }

   template <typename DT>
   void rng_openssl<DT>::rand
   (
		const uint64_t num,
		std::vector<DT>& rands
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
		MPMT_ASSERT(lb <= ub, "invalid input, lower bound (LB) is greater than upper bound (UB).");
	
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
   std::vector<DT> rng_openssl<DT>::rand
   (
		const DT lb,
		const DT ub,
		const uint64_t num
   ) const
   {
		std::vector<DT> result;

		return result;  // 移动语义
   }

   template <typename DT>
   void rng_openssl<DT>::rand
   (
		const DT lb,
		const DT ub,
		const uint64_t num,
		std::vector<DT>& rands
   ) const
   {
		if (rands.size() != num)
		{
			// resize 大小
			// 发出警告
		}
   }
}