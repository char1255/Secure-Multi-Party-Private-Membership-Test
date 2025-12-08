#include "core/mpmtcfg.hpp"
#include <cstdint>
#include <iostream>
#include <openssl/rand.h>
#include <ostream>
#include <string>
#include <chrono>
#include <vector>



using ring64 = uint64_t;
using size64 = uint64_t;

struct rng_array
{
    explicit rng_array(const size64 size)
        :
        m_data(size == 0 ? nullptr : std::make_unique<ring64[]>(size)),
        m_size(size)
    {}

    explicit rng_array(const uint64_t size, const ring64 default_value)
        : rng_array(size)
    {
        if (m_size != 0) {
            std::fill(m_data.get(), m_data.get() + m_size, default_value);
        }
    }

    rng_array() = delete;
    rng_array(const rng_array&) = delete;
    rng_array& operator=(const rng_array&) = delete;

    rng_array(rng_array&&) noexcept = default;
    rng_array& operator=(rng_array&&) noexcept = default;

    std::unique_ptr<ring64[]> m_data;
    uint64_t m_size;
};

namespace test
{
    static constexpr uint64_t mc_BLOCK_SIZE = 1ULL << 25; // 每次生成的块长度

    // 无范围环上随机数
    inline ring64 rand()
    {
        ring64 result;
        RAND_bytes((unsigned char*)&result, sizeof(ring64));
        return result;
    }

    // 批量无范围环上随机数
    inline rng_array rand(const size64 size)
    {
      	rng_array result(size);
        const size64 mnum  = sizeof(ring64) * size / mc_BLOCK_SIZE;
        const size64 rsize = sizeof(ring64) * size % mc_BLOCK_SIZE;
        size64 i = 0;
        unsigned char* buf = (unsigned char*)result.m_data.get();
        for(; i < mnum; ++i)
        {
            RAND_bytes(buf + i * mc_BLOCK_SIZE, mc_BLOCK_SIZE);
        }
        if (rsize != 0)
        {
            RAND_bytes(buf + i * mc_BLOCK_SIZE, rsize);
        }
        return result;
    }

    // 有范围随机数
    inline ring64 rand(const ring64 lb, const size64 ub)
    {
		MPMT_ASSERT(lb <= ub, "invalid input, lower bound (LB) is greater than upper bound (UB).");

        ring64 result;
        const ring64 maxv = ~(0ULL) ;
		if (ub == maxv && lb == 0)
		{
			return rand();
		}

		const ring64 range = (ub - lb) + 1;
		if (range == 1)
		{
			return lb;
		}

		const ring64 threshold = maxv - (maxv % range);

        do
        {
            RAND_bytes((unsigned char*)&result, sizeof(ring64));
        }
        while(result >= threshold);

        return (result % range) + lb;
    }

    // 批量有范围随机数：一次遍历，逐个拒绝采样矫正
    inline rng_array rand_1(const ring64 lb, const size64 ub, const size64 size)
    {
      	rng_array result(size);
        
        return result;
    }

    // 批量有范围随机数：滑动窗口

    // 批量有范围随机数：单独调用

}


int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    std::cout << "Secure Multi-Party Private Membership Test\n";



}