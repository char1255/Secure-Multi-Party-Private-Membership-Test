#ifndef RNG_HPP
#define RNG_HPP

#include <stdexcept>

/** @namespace 项目命名空间 */
namespace mpmt
{
    class rng_exc : public std::runtime_error
    {
    public:
        explicit rng_exc(const std::string& info) : std::runtime_error("random number generator exception: n: " + info) {}
    };
}
#endif // !RNG_HPP
