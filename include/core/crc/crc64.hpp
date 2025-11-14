#ifndef CRC64_HPP
#define CRC64_HPP

#include <cstdint>
#include <vector>
#include "core/ring/ring.hpp"


/** @namespace 项目命名空间 */
namespace mpmt
{
    class crc64
    {
    public:
        virtual uint64_t compute(const uint8_t* const data, const size_t len) = 0;
    };
}
#endif // !CRC64_HPP
