#include "core/mpmtcfg.hpp"
#include "core/crc/crc64_factory.hpp"
#include "core/crc/crc64_impl/crc64_ecma182.hpp"
#include "core/crc/crc64_impl/crc64_iso.hpp"

namespace mpmt
{
    // 假设 'choice' 参数已在调用层经过有效性验证。
    // 此工厂方法根据已知的 crc64_standard 类型返回对应的具体实现。
    std::unique_ptr<mpmt::crc64> crc64_factory::acquire_interface(const crc64_standard& choice)
    {
        switch (choice)
        {
        case crc64_standard::ECMA182:
            return std::make_unique<crc64_ecma182>();

        case crc64_standard::ISO:
            return std::make_unique<crc64_iso>();

        default:
            MPMT_WARN(false, "Undefined crc64_standard.");    
            return nullptr;
        }
    }
}