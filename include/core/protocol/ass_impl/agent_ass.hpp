
#ifndef DATA_HOLDER_ASS_HPP
#define DATA_HOLDER_ASS_HPP

#include "core/protocol/agent_ideal_fn.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    class agent_ass : public agent_ideal_fn 
    {
        virtual void merge() = 0;  
        virtual void update() = 0;  
        virtual void aggregate() = 0;  
    };
}
#endif // !DATA_HOLDER_ASS_HPP