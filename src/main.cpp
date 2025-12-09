#include <cstdint>
#include <memory>
#include <openssl/rand.h>
#include <nlohmann/json.hpp>
#include <indicators/progress_bar.hpp>
#include <chrono>
#include <vector>
#include <fstream>


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
    inline ring64 rand(const ring64 lb, const ring64 ub)
    {
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
    inline rng_array rand_batch(const ring64 lb, const ring64 ub, const size64 size)
    {
        const ring64 maxv = ~(0ULL) ;
		if (ub == maxv && lb == 0)
		{
			return rand(size);
		}

		const ring64 range = (ub - lb) + 1;
		if (range == 1)
		{
			return rng_array(size, lb);
		}

      	rng_array result(size);
		const ring64 threshold = maxv - (maxv % range);
        ring64* arr = result.m_data.get();
        const size64 mnum  = sizeof(ring64) * size / mc_BLOCK_SIZE;
        const size64 rsize = sizeof(ring64) * size % mc_BLOCK_SIZE;
        size64 i = 0;
        unsigned char * buf = (unsigned char*) arr;
        for(; i < mnum; ++i)
        {
            RAND_bytes(buf + i * mc_BLOCK_SIZE, mc_BLOCK_SIZE);
        }
        if (rsize != 0)
        {
            RAND_bytes(buf + i * mc_BLOCK_SIZE, rsize);
        }

        // 拒绝采样
        for(size64 idx = 0; idx < size; ++idx)
        {
			while(arr[idx] >= threshold)
            {
                RAND_bytes((unsigned char*)&arr[idx], sizeof(ring64));
            }
			arr[idx] = (arr[idx] % range) + lb;

        }
        return result;
    }

    // 批量有范围随机数：滑动窗口
    inline rng_array rand_window(const ring64 lb, const ring64 ub, const size64 size)
    {
        const ring64 maxv = ~(0ULL) ;
		if (ub == maxv && lb == 0)
		{
			return rand(size);
		}

		const ring64 range = (ub - lb) + 1;
		if (range == 1)
		{
			return rng_array(size, lb);
		}

		const ring64 threshold = maxv - (maxv % range);

        rng_array result(size);
        ring64* arr = result.m_data.get();
        const size64 window_size = mc_BLOCK_SIZE / sizeof(ring64);
        std::unique_ptr<ring64[]> rngbuf_unique = std::make_unique<ring64[]>(window_size);
        ring64* rngbuf = rngbuf_unique.get();

        size64 idx = 0;
        for(size64 k = 0; idx < size; k = (k+1) % window_size)
        {
            if(k == 0)
            {
                RAND_bytes((unsigned char*)rngbuf, window_size * sizeof(ring64));
            }

            if(rngbuf[k] >= threshold)
            {
                continue;
            }
   
            arr[idx] = (rngbuf[k] % range) + lb;
            ++idx;
        }
        return result;
    }
    
    // 批量有范围随机数：单独调用
    inline rng_array rand_call(const ring64 lb, const ring64 ub, const size64 size)
    {
        const ring64 maxv = ~(0ULL) ;
		if (ub == maxv && lb == 0)
		{
			return rand(size);
		}

		const ring64 range = (ub - lb) + 1;
		if (range == 1)
		{
			return rng_array(size, lb);
		}

        rng_array result(size);
        ring64* arr = result.m_data.get();
        for(size64 i = 0; i < size; ++i)
        {
            arr[i] = rand(lb, ub);
        }

        return result;
    }

    // 结合window与batch
    inline rng_array rand_best(const ring64 lb, const ring64 ub, const size64 size)
    {
        const ring64 maxv = ~(0ULL) ;
		if (ub == maxv && lb == 0)
		{
			return rand(size);
		}

		const ring64 range = (ub - lb) + 1;
		if (range == 1)
		{
			return rng_array(size, lb);
		}

      	rng_array result(size);
		const ring64 threshold = maxv - (maxv % range);
        ring64* arr = result.m_data.get();
        const size64 mnum  = sizeof(ring64) * size / mc_BLOCK_SIZE;
        const size64 rsize = sizeof(ring64) * size % mc_BLOCK_SIZE;
        size64 i = 0;
        unsigned char * buf = (unsigned char*) arr;
        for(; i < mnum; ++i)
        {
            RAND_bytes(buf + i * mc_BLOCK_SIZE, mc_BLOCK_SIZE);
        }
        if (rsize != 0)
        {
            RAND_bytes(buf + i * mc_BLOCK_SIZE, rsize);
        }

        const size64 window_size = mc_BLOCK_SIZE / sizeof(ring64);
        std::unique_ptr<ring64[]> rngbuf_unique = std::make_unique<ring64[]>(window_size);
        ring64* rngbuf = rngbuf_unique.get();
        size64 winidx = 0;

        // 拒绝采样
        for(size64 idx = 0; idx < size; ++idx)
        {
			while(arr[idx] >= threshold)
            {
                if(winidx == 0)
                {
                    RAND_bytes((unsigned char*)rngbuf, window_size * sizeof(ring64));                
                }
                arr[idx] = rngbuf[winidx];
                winidx = (winidx + 1) % window_size;
            }
			arr[idx] = (arr[idx] % range) + lb;
        }

        return result;  
    }

}

struct test_res
{
    long long res_bench;
    long long res_batch;
    long long res_window;
    long long res_call;
    long long res_best;
    ring64 checksum;
};

static const size64 test_times = 30;

test_res test_script(const size64 rngsize, const ring64 lb, const ring64 ub)
{
    long long res_bench = 0LL;
    long long res_batch = 0LL;
    long long res_window = 0LL;
    long long res_call = 0LL;
    long long res_best = 0LL;

    volatile ring64 check_sum_bench = 0;
    volatile ring64 check_sum_batch = 0;
    volatile ring64 check_sum_window = 0;
    volatile ring64 check_sum_call = 0;
    volatile ring64 check_sum_best = 0;

    // --- 0. 无范围基准 ---
    for(size64 t = 0; t < test_times; ++t)
    {
        auto start_bench = std::chrono::steady_clock::now();
        
        rng_array result = test::rand(rngsize);
        
        auto end_bench = std::chrono::steady_clock::now();

        ring64 current_sum = 0;
        for (size64 i = 0; i < rngsize; ++i) { 
            current_sum += result.m_data[i];
        }
        check_sum_bench += current_sum; 
        
        res_bench += 
            std::chrono::duration_cast<std::chrono::microseconds>(end_bench - start_bench).count();
    }
    
    // --- 1. rand_batch 测试 ---
    for(size64 t = 0; t < test_times; ++t)
    {
        auto start_batch = std::chrono::steady_clock::now();
        
        rng_array result = test::rand_batch(lb, ub, rngsize);
        
        auto end_batch = std::chrono::steady_clock::now();

        ring64 current_sum = 0;
        for (size64 i = 0; i < rngsize; ++i) { 
            current_sum += result.m_data[i];
        }
        check_sum_batch += current_sum; 
        
        res_batch += 
            std::chrono::duration_cast<std::chrono::microseconds>(end_batch - start_batch).count();
    }
    
    // --- 2. rand_window 测试 ---
    for(size64 t = 0; t < test_times; ++t)
    {
        auto start_window = std::chrono::steady_clock::now();
        
        rng_array result = test::rand_window(lb, ub, rngsize);
        
        auto end_window = std::chrono::steady_clock::now();
        
        ring64 current_sum = 0;
        for (size64 i = 0; i < rngsize; ++i) {
            current_sum += result.m_data[i];
        }
        check_sum_window += current_sum;
        
        res_window += 
            std::chrono::duration_cast<std::chrono::microseconds>(end_window - start_window).count();
    }

    // --- 3. rand_call 测试 ---
    for(size64 t = 0; t < test_times; ++t)
    {
        auto start_call = std::chrono::steady_clock::now();
        
        rng_array result = test::rand_call(lb, ub, rngsize);
        
        auto end_call = std::chrono::steady_clock::now();
        
        ring64 current_sum = 0;
        for (size64 i = 0; i < rngsize; ++i) {
            current_sum += result.m_data[i];
        }
        check_sum_call += current_sum;
        
        res_call += 
            std::chrono::duration_cast<std::chrono::microseconds>(end_call - start_call).count();
    }

    // --- 4. rand_best 测试 ---
    for(size64 t = 0; t < test_times; ++t)
    {
        auto start_best = std::chrono::steady_clock::now();
        
        rng_array result = test::rand_best(lb, ub, rngsize);
        
        auto end_best = std::chrono::steady_clock::now();
        
        ring64 current_sum = 0;
        for (size64 i = 0; i < rngsize; ++i) {
            current_sum += result.m_data[i];
        }
        check_sum_best += current_sum;
        
        res_best += 
            std::chrono::duration_cast<std::chrono::microseconds>(end_best - start_best).count();
    }

    test_res result;
    result.res_bench  = res_bench;
    result.res_batch  = res_batch;
    result.res_call   = res_call;
    result.res_best   = res_best;
    result.res_window = res_window;
    result.checksum   = check_sum_batch + check_sum_window + check_sum_call + check_sum_best;
    return  result;
}

int main(int argc, char** argv)
{
    (void)argc; (void)argv;

    std::vector<size64> rng_size = 
    {
        1ULL << 16,1ULL << 20,1ULL << 22, 1ULL << 24,1ULL << 26, 1ULL << 28
    };

    std::vector<size64> range_size = 
    {
        1ULL << 16,1ULL << 20,1ULL << 22, 1ULL << 24,1ULL << 26, 1ULL << 28
    };

    size64 total_steps = rng_size.size() * range_size.size();

    indicators::ProgressBar bar{
        indicators::option::BarWidth{50},
        indicators::option::Start{"["},
        indicators::option::Fill{"="},
        indicators::option::Lead{">"},
        indicators::option::Remainder{" "},
        indicators::option::ShowPercentage{true}
    };
    
    size64 current_step = 0;

    std::vector<std::vector<long long>> rrbench(rng_size.size(), std::vector<long long>(range_size.size(), 0));
    std::vector<std::vector<long long>> rrbatch(rng_size.size(), std::vector<long long>(range_size.size(), 0));
    std::vector<std::vector<long long>> rrwindow(rng_size.size(), std::vector<long long>(range_size.size(), 0));
    std::vector<std::vector<long long>> rrcall(rng_size.size(), std::vector<long long>(range_size.size(), 0));
    std::vector<std::vector<long long>> rrbest(rng_size.size(), std::vector<long long>(range_size.size(), 0));

    for(size64 i = 0; i < rng_size.size(); ++i)
    {
        for(size64 j = 0; j < range_size.size(); ++j)
        {
            test_res tmpres = test_script(rng_size[i], 0, range_size[j] - 1);
            rrbench[i][j] = tmpres.res_bench;
            rrbatch[i][j] = tmpres.res_batch;
            rrwindow[i][j] = tmpres.res_window;
            rrcall[i][j] = tmpres.res_call;
            rrbest[i][j] = tmpres.res_best;
            ++current_step;
            bar.set_progress(static_cast<size_t>(current_step * 100 / total_steps));
        }
    }

    bar.set_progress(100);
    std::cout << std::endl;

    nlohmann::json j;
    j["bench_time"] = rrbench;
    j["batch_time"] = rrbatch;
    j["window_time"] = rrwindow;
    j["call_time"] = rrcall;
    j["best_time"] = rrbest;
    j["range"] = range_size;
    j["rngsize"] = rng_size;

    std::ofstream outfile("F:/Fall Semester 2025-2026/data.json");
    outfile << j.dump(4);
    outfile.close();
}
