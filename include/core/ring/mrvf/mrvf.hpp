#ifndef MRVF_HPP
#define MRVF_HPP

#include "core/ring/rvector.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    template<typename RT>
    struct mrvf : mrvf_info<RT>
    {
        static_assert(
            is_ring_type,
            "RT must be ring1, ring8, ring16, ring32 or ring64."
            );

        mrvf(mpmt::rvector<RT>&& rvec_to_move) noexcept
            :
            mc_ring_size(sizeof(RT)),
            m_rvector(std::move(rvec_to_move))
        {}

        const uint8_t mc_ring_size;     // 环大小
        mpmt::rvector<RT> m_rvector;    // 向量
    };
}

#endif // !MRVF_HPP