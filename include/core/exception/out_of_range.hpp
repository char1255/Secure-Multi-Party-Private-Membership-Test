#ifndef OUT_OFF_RANGE_HPP
#define OUT_OFF_RANGE_HPP

#include <stdexcept>

/** @namespace 项目命名空间 */
namespace mpmt
{
    class out_of_range : public std::out_of_range
    {
        
    };
}
#endif // !OUT_OFF_RANGE_HPP
