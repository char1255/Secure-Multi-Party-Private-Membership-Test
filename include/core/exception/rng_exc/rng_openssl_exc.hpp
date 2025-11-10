#ifndef RNG_OPENSSL_HPP
#define RNG_OPENSSL_HPP

#include <stdexcept>
#include "core/exception/rng_exc/rng_exc.hpp"


/** @namespace 项目命名空间 */
namespace mpmt
{
    class rng_openssl_exc : public rng_exc
    {
    public:
        enum class error_code
        {
            RANDOM_ENGINE_ERROR,    // 随机数系统内部错误：熵不足，随机源丢失等等
            INVALID_INPUT           // 错误的输入
        };

        explicit rng_openssl_exc
        (
            const std::string& info, 
            error_code code
        ) 
        : rng_exc(info),
            _code(code)
        {}
        error_code get_error_code() const { return _code; }

    private:
        error_code _code;
    };
}
#endif // !RNG_OPENSSL_HPP
