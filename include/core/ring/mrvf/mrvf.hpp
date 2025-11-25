#ifndef MRVF_HPP
#define MRVF_HPP

#include "core/ring/rvector.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    template<typename RT>
    struct mrvf_info
    {
        static_assert(
            is_ring_type,
            "RT must be ring1, ring8, ring16, ring32 or ring64."
            );

        // 通过参数直接获取，在载入时会通过静态断言确保 ring_size==sizeof(RT)
        mrvf_info() :mc_ring_size(sizeof(RT))
        {}

        const uint8_t mc_ring_size; // 环大小
    };

    template<typename RT>
    struct mrvf : mrvf_info<RT>
    {
        mrvf(mpmt::rvector<RT>&& rvec_to_move) noexcept
            :
            mrvf_info(),
            m_rvector(std::move(rvec_to_move))
        {}

        mpmt::rvector<RT> m_rvector;    // 向量
    };
}

#endif // !MRVF_HPP