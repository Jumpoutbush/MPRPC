#pragma once
#include "const.h"
#include "google/protobuf/service.h"
#include "mprpcapplication.h"
#include "zookeeperutil.h"

#include <google/protobuf/descriptor.h>
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
    // 事件循环
    muduo::net::EventLoop m_eventloop; 

    // 服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *m_service;   // 服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; // <方法名，方法描述>
    };
    std::unordered_map<std::string, ServiceInfo> m_servicesMap; // 注册成果的<服务名，服务信息>

    // tcpserver
    // 新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr &conn);
    // 接收到新的rpc请求消息
    void OnMessage(const muduo::net::TcpConnectionPtr &conn, 
                muduo::net::Buffer *buf, muduo::Timestamp time);
    // Closure回调
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message* response);
};

}