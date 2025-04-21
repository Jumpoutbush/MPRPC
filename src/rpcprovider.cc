
#include "rpcprovider.h"
#include <functional>

namespace mprpc{
void RpcProvider::NotifyService(google::protobuf::Service *service)
{

}

void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetConfig().GetConfigValue("rpcserverip");
    uint16_t port = stoi(MprpcApplication::GetConfig().GetConfigValue("rpcserverport"));
    muduo::net::InetAddress m_addr(ip, port);

    muduo::net::TcpServer server(&m_eventloop, m_addr , "RpcProvider");

    // 绑定连接回调和消息读写回调
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 设置muduo的线程数量
    server.setThreadNum(4); // io + worker * 3

    // 启动服务
    server.start();
    m_eventloop.loop();

}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{

}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, 
    muduo::net::Buffer *buf, muduo::Timestamp time)
{

}

}