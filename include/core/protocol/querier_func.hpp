#ifndef QUERIER_FUNC_HPP
#define QUERIER_FUNC_HPP

#include "core/mpmtcfg.hpp"
#include "core/protocol/base_func.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    class querier_func : public base_func    // 查询方理想功能（接口）
    {
    public:
        virtual void query() = 0;   // 数据持有方额外具有查询接口
    };
}
#endif // !QUERIER_FUNC_HPP