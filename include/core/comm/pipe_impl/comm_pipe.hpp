#ifndef COMM_HPP
#define COMM_HPP

#include "core/comm/comm_adapter.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    /**
     * @class   用管道实现的通信适配器
     * @tparam  DT 传输数据类型，限定为ring32,ring64,size_t
     */
    template <typename DT>
    class comm_pipe : public comm_adapter<DT>
    {
        /** @brief 断言限制模板类型 */
        static_assert(
            std::is_same_v<DT, ring8>
            || std::is_same_v<DT, ring16> 
            || std::is_same_v<DT, ring32> 
            || std::is_same_v<DT, ring64>
            || std::is_same_v<DT, size_t>,
            "DT must be ring8, ring16, ring32, ring64 or size_t."
        );
    };
}
#include "core/comm/pipe_impl/comm_pipe.tpp"

#endif // !COMM_HPP