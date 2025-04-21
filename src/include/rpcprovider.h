#pragma once
#include "const.h"
#include "google/protobuf/service.h"
#include "mprpcapplication.h"

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
namespace mprpc{
/** 
    @brief 框架基础类
*/
class RpcProvider 
{
public:
    /**
     * @brief 框架提供给外部使用的，可以发布rpc方法的接口
     */
    void NotifyService(google::protobuf::Service *service);

    /**
     * @brief 启动rpc服务节点，开始提供rpc远程网络调用服务
     */
    void Run();
private:
    // 新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr &conn);
    // 接收到新的rpc请求消息
    void OnMessage(const muduo::net::TcpConnectionPtr &conn, 
                muduo::net::Buffer *buf, muduo::Timestamp time);

    // 事件循环
    muduo::net::EventLoop m_eventloop; 
};

}