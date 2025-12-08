#include <iostream>
#include "core/mpmtcfg.hpp"
int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    std::cout << "Secure Multi-Party Private Membership Test\n";
    MPMT_WARN(false, "genshin start");
    MPMT_ASSERT(false, "genshin start");
}
