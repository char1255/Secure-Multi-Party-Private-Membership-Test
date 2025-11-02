#ifndef COMM_HPP
#define COMM_HPP

#include "core/infrastructure/communication/comm_adapter.hpp"

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
            std::is_same<DT, ring32>::value || std::is_same<DT, ring64>::value || std::is_same<DT, size_t>::value,
            "DT must be one of: ring32, ring64, size_t");
    };
}

#endif // !COMM_HPP