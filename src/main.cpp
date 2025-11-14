#include <iostream>
#include <chrono>
#include <string>
#include "core/ring/rvector.hpp"
#include "core/ring/ring.hpp"
#include <iomanip>

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    // std::cout << "Secure Multi-Party Private Membership Test\n";

    const uint64_t polynomial = 0x42F0E1EBA9EA3693;
    std::cout << "static inline uint64_t m_table[256] = {\n";

    for (uint64_t i = 0; i < 256; i++)
    {
        uint64_t crc = i;
        for (uint64_t j = 0; j < 8; j++)
        {
            if (crc & 1)
            {
                crc = (crc >> 1) ^ polynomial;
            }
            else
            {
                crc = crc >> 1;
            }
        }
        std::cout 
            << "0x"
            << std::hex << std::uppercase
            << std::setw(16)
            << std::setfill('0')
            << crc
            << "ULL"
            ;

        if (i != 255)
            std::cout << ", ";
        if(i%4 == 3){
            std::cout << "\n";
        }
    }
    std::cout << "};";

    return 0;
}