#ifndef MPMTCFG_HPP
#define MPMTCFG_HPP

#include <cstdint>

// 自定义断言宏：在 debug ver.中，使用断言；在release ver.中使用错误抛出。

/** @namespace 项目命名空间。 */
namespace mpmt{

    /** @typedef 1位环数 (boolean)。 */
    using ring1  = uint8_t;

    /** @typedef 32位环数。 */
    using ring32 = uint32_t;

    /** @typedef 64位环数。 */
    using ring64 = uint64_t;   

}
#endif // !MPMTCFG_HPP