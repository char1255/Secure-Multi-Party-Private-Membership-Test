#ifndef OUT_OFF_RANGE_HPP
#define OUT_OFF_RANGE_HPP

#include <stdexcept>

/** @namespace 项目命名空间 */
namespace mpmt
{
    class rng_exc : public std::runtime_error
    {
    public:
        enum class impl_type
        {
            openssl
        };

        explicit rng_exc(const std::string &info, impl_type type)
            : std::runtime_error("RngException: " + info), _type(type) {}
        
            impl_type get_impl_type() const { return _type; }

    private:
        impl_type _type;
    };
}
#endif // !OUT_OFF_RANGE_HPP
