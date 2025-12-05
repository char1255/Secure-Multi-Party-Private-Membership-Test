#include <string>    
#include <limits>
#include "core/mpmtcfg.hpp"
#include "core/exception/rng_exc.hpp"
#include "core/rng/rng_adapter.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
	template <typename DT>
	DT rng_openssl<DT>::rand() const
	{
		DT r = 0;
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
	rng_array<DT> rng_openssl<DT>::rand(const uint32_t size) const
	{
      	rng_array<DT> result(size);

		if (result.m_size != 0)
		{		
			if (RAND_bytes((unsigned char*)result.m_data.get(), sizeof(DT) * result.m_size) != 1)
			{
				throw mpmt::rng_exc
				(
					rng_exc::impl_type::OPENSSL,
					"random number generation failed, low entropy or internal error."
				);
			}
		}

		return result;
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

		DT r = 0;
		const DT threshold = maxv - (maxv % range);
		do
		{
			if (RAND_bytes((unsigned char*)&r, sizeof(DT)) != 1)
			{
				throw mpmt::rng_exc
				(
					rng_exc::impl_type::OPENSSL,
					"random number generation failed, low entropy or internal error."
				);
			}
		} while (r >= threshold);

		return (r % range) + lb;
   	}

	template <typename DT>
	rng_array<DT> rng_openssl<DT>::rand
	(
		const DT lb,
		const DT ub,
		const uint32_t size
	) const
	{
		MPMT_ASSERT(lb <= ub, "invalid input, lower bound (LB) is greater than upper bound (UB).");

		const DT maxv = std::numeric_limits<DT>::max();
		if (ub == maxv && lb == 0)
		{
			return rand(size);
		}

		const DT range = (ub - lb) + 1;
		if (range == 1)
		{
			return rng_array<DT>(size, lb);
		}

		rng_array<DT> result(size);
		if (result.m_size != 0)
		{		
			DT* arr = result.m_data.get(); 
			if (RAND_bytes((unsigned char*)arr, sizeof(DT) * result.m_size) != 1)
			{
				throw mpmt::rng_exc
				(
					rng_exc::impl_type::OPENSSL,
					"random number generation failed, low entropy or internal error."
				);
			}
	
			const DT threshold = maxv - (maxv % range);
			for (uint32_t i = 0; i < result.m_size; ++i)
			{
				while(arr[i] >= threshold)
				{
					if (RAND_bytes((unsigned char*)&arr[i], sizeof(DT)) != 1)
					{
						throw mpmt::rng_exc
						(
							rng_exc::impl_type::OPENSSL,
							"random number generation failed, low entropy or internal error."
						);
					}
				}
				arr[i] = (arr[i] % range) + lb;
			}
		}
		return result;
    }
}