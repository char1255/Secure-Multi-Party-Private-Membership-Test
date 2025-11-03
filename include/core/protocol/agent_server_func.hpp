#ifndef DATA_HOLDER_FUNC_HPP
#define DATA_HOLDER_FUNC_HPP

#include "core/mpmtcfg.hpp"
#include "core/protocol/base_func.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    class agent_server_func : public base_func // 代理服务器理想功能（接口）
    {
    public:
        // 代理服务器额外具有如下接口：
        virtual void merge() = 0;
        virtual void update() = 0;
        virtual void aggregate() = 0;

    private:
        virtual void multiply() = 0;
    };
}
#endif // !DATA_HOLDER_FUNC_HPP