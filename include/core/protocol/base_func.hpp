#ifndef BASE_FUNC_HPP
#define BASE_FUNC_HPP

#include "core/mpmtcfg.hpp"
#include "core/ring/ring.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    class base_func // 基础理想功能（接口）
    {
    public:
        virtual void share() = 0;       // 秘密分享功能
        virtual void reveal() = 0;      // 恢复秘密功能
    };
}
#endif // !BASE_FUNC_HPP