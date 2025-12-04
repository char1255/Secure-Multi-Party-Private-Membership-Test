#include <iostream>
#include <xsimd/xsimd.hpp>
#include "core/mpmtcfg.hpp"
#include "core/ring/mrvf/mrvf_handler.hpp"
#include "core/ring/ring.hpp"

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    std::cout << "Secure Multi-Party Private Membership Test\n";
  
    MPMT_WARN(false, "genshin start");
    MPMT_ASSERT(false, "genshin start");

    mpmt::mrvf_handler<mpmt::ring32>::config cfg = 
    {
      false,
      false
    };

    mpmt::mrvf_handler<mpmt::ring32> handler(cfg);
    handler.load("");
    return 0;
}