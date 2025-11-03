#ifndef RING_HPP
#define RING_HPP
#include <cstdint>

/** @namespace 项目命名空间。*/
namespace mpmt{
    /** @typedef 1位环。*@note 仅仅在临时从rvector获取值时使用，实际大小是8bit。*/
    using ring1 = uint8_t;

    /** @typedef 8位环 */
    using ring8 = uint8_t;

    /** @typedef 16位环 */
    using ring16 = uint16_t;

    /** @typedef 32位环。 */
    using ring32 = uint32_t;

    /** @typedef 64位环。 */
    using ring64 = uint64_t;  
    
    // template<typename RT>
    // boolean_to_arithmetic(){
        
    // }

}
#endif // !RING_HPP