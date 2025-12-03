#ifndef STACK_TRACER_HPP
#define STACK_TRACER_HPP

#include <cstdio>

namespace utils
{
    /**
     * @class 堆栈打印工具
     */
    class stack_tracer
    {
    public:
        static void print(std::size_t max_frames = 32) noexcept;
        
    };
}

#endif // STACK_TRACER_HPP
