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

        enum class error_code
        {
            RANDOM_ENGINE_ERROR, // 随机数系统内部错误：熵不足，随机源丢失等等
            INVALID_INPUT        // 错误的输入
        };

        explicit rng_exc(const std::string &info, impl_type type, error_code code) : std::runtime_error("RngException: " + info),
                                                                                     _type(type),
                                                                                     _code(code)
        {
        }
        impl_type get_impl_type() const { return _type; }
        error_code get_error_code() const { return _code; }

    private:
        impl_type _type;
        error_code _code;
    };
}
#endif // !OUT_OFF_RANGE_HPP
