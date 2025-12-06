#include <iostream>
#include <vector>
// #include "core/mpmtcfg.hpp"
// int main(int argc, char** argv)
// {
//     (void)argc; (void)argv;
//     std::cout << "Secure Multi-Party Private Membership Test\n";
//     MPMT_WARN(false, "genshin start");
//     MPMT_ASSERT(false, "genshin start");
// }

using idx64 = uint64_t;

std::vector<idx64> rand_permutations(const idx64 lb, const idx64 ub);

int main()
{
    const idx64 l1 = 5;
    const idx64 l2 = 7;
    std::vector<idx64> L1(l1);      // 这里只做逻辑表示，不去赋值
    std::vector<idx64> L2(l2);      // 这里只做逻辑表示，不去赋值
    std::vector<idx64> pi1(l1 + l2);
    std::vector<idx64> pi2(l1 + l2);

    // step1: 生成随机置换
    std::vector<idx64> pi = rand_permutations(0, l1 + l2 - 1);

    // step2: 生成 pi1
    for(idx64 i = 0; i < l1; ++i)
    {
        bool find = false;
        for(idx64 j = 0; j < l2; ++j)
        {
            if(L1[i] == L2[j])
            {
                pi1[i] = pi[j];
                find = true;
                break;
            }
        }
        if(!find) pi1[i] = pi[l2 + i];
    }

    // 哪些 idx在 pi中未被访问，全部随机地排列，然后填充到 pi1的 l1 ~ (l1+l2-1)

    // 分发pi2
    pi2 = pi;

    return 0;
}