#ifndef RNG_EXC_HPP
#define RNG_EXC_HPP

#include <stdexcept>
#include <string>
#include "core/mpmtcfg.hpp"

/** @namespace 项目命名空间 */
namespace mpmt
{
    class rng_exc : public std::runtime_error
    {
    public:
        enum class impl_type
        {
            OPENSSL,
            CPPSTL
        };

        explicit rng_exc
        (
            const impl_type& type,
            const std::string& info
        ) :
            m_type(type),
            std::runtime_error(build_message(type, info))
        {}
    private:
        impl_type m_type;

        static std::string build_message(impl_type type, const std::string& info)
        {
            switch (type)
            {
            case impl_type::OPENSSL:    return "RNG OpenSSL exception: " + info;
            case impl_type::CPPSTL:     return "RNG C++ STL exception: " + info;
            default:
                MPMT_WARN(false, "Undefined rng_exc::impl_type.");
                return "RNG Unknown exception: " + info;
            }
        }
    };
}
#endif // !RNG_EXC_HPP
