#ifndef CRC64_FACTORY_HPP
#define CRC64_FACTORY_HPP

#include <memory>
#include "core/crc/crc64.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    class crc64_factory
    {
    public:
        enum class standard
        {
            ECMA182,
            ISO
        };

        static std::unique_ptr<mpmt::crc64> acquire_interface(const standard& choice);
    };
}

#endif // !CRC64_FACTORY_HPP