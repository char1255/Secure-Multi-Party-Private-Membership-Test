#include <iostream>
#include <chrono>
#include "core/ring/rvector.hpp"
#include "core/ring/ring.hpp"

int main(int argc, char** argv)
{
    const std::size_t vector_size = 1e7;

    mpmt::rvector<mpmt::ring32> a(vector_size);
    mpmt::rvector<mpmt::ring32> b(vector_size);

    auto start = std::chrono::high_resolution_clock::now();
    

    for(size_t i = 0; i < 10;++i){
        a += b;
    }

    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Secure Multi-Party Private Membership Test\n";
    std::cout << duration_ms.count() << " milliseconds\n";

    (void)argc; (void)argv;
    return 0;
}