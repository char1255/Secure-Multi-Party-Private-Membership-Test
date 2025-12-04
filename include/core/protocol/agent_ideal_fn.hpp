#ifndef DATA_HOLDER_FUNC_HPP
#define DATA_HOLDER_FUNC_HPP

#include "core/protocol/base_ideal_fn.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    class agent_ideal_fn : public base_ideal_fn // 代理方理想功能（接口）
    {
    public:
        virtual void merge() = 0;
        virtual void update() = 0;
        virtual void aggregate() = 0;

    private:
        virtual void multiply() = 0;
        virtual void subtract() = 0;
        virtual void add() = 0;
    };
}
#endif // !DATA_HOLDER_FUNC_HPP