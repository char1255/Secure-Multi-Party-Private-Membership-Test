#include <iostream>
#include <chrono>
#include "core/ring/rvector.hpp"
#include "core/ring/ring.hpp"

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    // std::cout << "Secure Multi-Party Private Membership Test\n";

// 1. 获取四个字符的 uint8_t 整数值
    uint32_t byte4 = static_cast<uint32_t>('M'); // 0x4D (77)
    uint32_t byte3 = static_cast<uint32_t>('P'); // 0x50 (80)
    uint32_t byte2 = static_cast<uint32_t>('M'); // 0x4D (77)
    uint32_t byte1 = static_cast<uint32_t>('T'); // 0x54 (84)

    uint32_t result_32bit = 0x4D504D54; // "MPMT" 

    std::cout << "32-bit Integer (Dec): "
        << std::dec                
        << result_32bit
        << std::endl;

    return 0;
}