#ifndef COMM_HPP
#define COMM_HPP

#include "core/comm/comm_adapter.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    /**
     * @class   用管道实现的通信适配器
     * @tparam  DT 传输数据类型，限定为ring32,ring64
     */
    template <typename DT>
    class comm_pipe : public comm_adapter<DT>
    {
    };
}
#include "core/comm/pipe_impl/comm_pipe.tpp"

#endif // !COMM_HPP