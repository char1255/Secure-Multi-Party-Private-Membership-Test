// #include <iostream>
// #include <chrono>
// #include <xsimd/xsimd.hpp>

// int main(int argc, char** argv)
// {
//     (void)argc; (void)argv;
//     // std::cout << "Secure Multi-Party Private Membership Test\n";

    
//     return 0;
// }


#include <stdint.h>
#include <stdio.h>

int main() {
    uint32_t x = 0x01020304;
    uint8_t *p = (uint8_t*)&x;

    if (p[0] == 0x04)
        printf("small\n");
    else if (p[0] == 0x01)
        printf("big\n");
    else
        printf("strange\n");
}
