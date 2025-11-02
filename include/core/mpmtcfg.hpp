#ifndef MPMTCFG_HPP
#define MPMTCFG_HPP

#include <cstdint>

#define VCB_STL     // Vector Computing Backend: C++STL
#define VCB_CUDA    // Vector Computing Backend: CUDA Thrust
#define VCB_XSIMD   // Vector Computing Backend: XSIMD

#define MPMT_ASSERT(cond, msg)

// 自定义断言宏：在 debug ver.中，使用断言；在release ver.中使用错误抛出。

/** @namespace 项目命名空间。 */
namespace mpmt{

    /** @typedef 1位环。*@note 仅仅在临时从rvector获取值时使用，实际大小是8bit。*/
    using ring1 = uint8_t;

    /** @typedef 8位环 */
    using ring8 = uint8_t;

    /** @typedef 16位环 */
    using ring16 = uint8_t;

    /** @typedef 32位环。 */
    using ring32 = uint32_t;

    /** @typedef 64位环。 */
    using ring64 = uint64_t;   

}
#endif // !MPMTCFG_HPP