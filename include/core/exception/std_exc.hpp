#ifndef STD_EXC_HPP
#define STD_EXC_HPP

#include <stdexcept>

/** @namespace 项目命名空间 */
namespace mpmt
{
    class std_exc : public std::runtime_error
    {
    public:
        enum class error_code
        {
            MEMORY_ALLOC_FAIL
        };

        explicit std_exc
        (
            const std::string& info, 
            error_code code
        ) : std::runtime_error("standard exception: " + info),
            _code(code)
        {}

        error_code get_error_code() const { return _code; }

    private:
        error_code _code;
    };
}
#endif // !STD_EXC_HPP
