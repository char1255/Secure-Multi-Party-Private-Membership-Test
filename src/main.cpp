#include <iostream>
#include <cstdint>
#include <vector>
#include <openssl/rand.h>
#include <chrono>

// 用 OpenSSL 生成均匀 32-bit 随机数
static inline uint32_t rand32() {
    uint32_t r;
    RAND_bytes(reinterpret_cast<unsigned char*>(&r), sizeof(r));
    return r;
}

// 测试某个 range 的 rejection sampling 平均循环次数和平均耗时
uint64_t test_range(uint32_t range, uint64_t iterations, double &avg_time_ns) {
    uint64_t loops = 0;
    uint32_t limit = UINT32_MAX - (UINT32_MAX % range);

    auto start = std::chrono::high_resolution_clock::now();

    for (uint64_t i = 0; i < iterations; ++i) {
        uint32_t v;
        do {
            v = rand32();
            ++loops;
        } while (v >= limit);
    }

    auto end = std::chrono::high_resolution_clock::now();
    avg_time_ns = std::chrono::duration<double, std::milli>(end - start).count() / iterations;

    return loops;
}

int main() {
    std::vector<uint32_t> test_ranges = {
        10U,
        100U,
        1000U,
        1000000U,
        1000000000U,
        (1U << 16),
        (1U << 24),
        (1U << 31),
        (1U << 31) - 1,
        UINT32_MAX - 1,
        UINT32_MAX
    };

    uint64_t iterations = 1000000; // 每个 range 测试 100 万次

    std::cout << "Range Size   | Avg Loops | Avg Time (ms)\n";
    std::cout << "-----------------------------------------\n";

    for (auto r : test_ranges) {
        double avg_time;
        uint64_t loops = test_range(r, iterations, avg_time);
        double avg_loops = static_cast<double>(loops) / iterations;
        std::cout << r << " | " << avg_loops << " | " << avg_time << "\n";
    }

    return 0;
}
