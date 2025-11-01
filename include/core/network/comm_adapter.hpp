#ifndef COMM_HPP
#define COMM_HPP

#include <type_traits>
#include <vector>

#include "core/mpmtcfg.hpp"

/** @namespace 项目命名空间。 */
namespace mpmt
{
    /**
     * @class   通信适配器，用于封装不同实现的通信接口。
     * @tparam  DT 传输数据类型，限定为ring32,ring64,size_t
     * @note    一个comm_adapter对象在同一时刻维护单独的一条连接，
     *          一些连接信息应通过实现类的构造函数赋值，并存储于实现类的成员变量中。
     */
    template <typename DT>
    class comm_adapter
    {
    public:
        /** @brief 断言限制模板类型 */
        static_assert(
            std::is_same<DT, ring32>::value || std::is_same_v<DT, ring64>::value || std::is_same_v<DT, size_t>::value,
            "DT must be one of: ring32, ring64, size_t");
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 以下为连接管理接口

        /**
         * @brief   与通信目标连接。
         * @note    通过成员变量标识的通信目标信息进行连接。
         */
        virtual void connect() = 0;

        /**
         * @brief   与通信目标断开连接。
         * @note    通过成员变量标识的通信目标信息断开连接。
         */
        virtual void disconnect() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // 以下为通信接口

        /**
         * @brief   向通信目标发送单个数据 (DT)。
         * @param   const DT send_number 发送数据。
         */
        virtual void send(const DT send_number) = 0;

        /**
         * @brief   从通信目标接收单个数据 (DT)。
         * @param   DT&  接收数据引用，用于写入访问。
         */
        virtual void receive(DT &recv_number) = 0;

        /**
         * @brief   向通信目标发送数据 (DT)。
         * @param   const std::vector<DT>& send_buf 发送缓存的引用，用于只读访问。
         * @note    需要保障send_buf不为空。
         */
        virtual void send(const std::vector<DT> &send_buf) = 0;

        /**
         * @brief   从通信目标接收数据 (DT)。
         * @param   std::vector<DT>& send_buf 发送缓存的引用，用于只读访问。
         * @note    需要保障send_buf为空，内存分配由该接口完成。
         */
        virtual void receive(std::vector<DT> &recv_buf) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //

        /**
         * @brief  析构通信接口
         */
        virtual ~comm_adapter() = 0;
    };

}

#endif // !COMM_HPP