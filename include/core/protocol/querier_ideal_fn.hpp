#ifndef QUERIER_FUNC_HPP
#define QUERIER_FUNC_HPP

#include "core/mpmtcfg.hpp"
#include "core/protocol/base_ideal_fn.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    class querier_ideal_fn : public base_ideal_fn    // 查询方理想功能（接口）
    {
    public:
        virtual void query() = 0;   // 数据持有方额外具有查询接口
    };
}
#endif // !QUERIER_FUNC_HPP