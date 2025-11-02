
#ifndef DATA_HOLDER_FSS_HPP
#define DATA_HOLDER_FSS_HPP

#include "core/mpmtcfg.hpp"
#include "core/protocol/agent_server_func.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    class agent_server_fss : public agent_server_func 
    {
        virtual void merge() = 0;  
        virtual void update() = 0;  
        virtual void aggregate() = 0;  
    };
}
#endif // !DATA_HOLDER_FSS_HPP