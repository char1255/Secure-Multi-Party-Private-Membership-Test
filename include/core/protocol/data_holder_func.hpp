#ifndef DATA_HOLDER_FUNC_HPP
#define DATA_HOLDER_FUNC_HPP

#include "core/mpmtcfg.hpp"
#include "core/protocol/base_func.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    class data_holder_func : public base_func    // 数据持有方理想功能（接口）
    {
        // 数据持有方不具备其他理想功能，仅做数据分发
    };
}
#endif // !DATA_HOLDER_FUNC_HPP