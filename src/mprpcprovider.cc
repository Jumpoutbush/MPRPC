
#include "mprpcprovider.h"
#include "rpcheader.pb.h"
#include <functional>

namespace mprpc{
// 注册服务
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;

    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();   // 服务描述信息
    std::string service_name = pserviceDesc->name();    // 服务名称
    std::uint16_t method_count = pserviceDesc->method_count();    // 方法数量
    std::cout << "service name: " << service_name << " method count: " << method_count << std::endl;

    for(int i = 0; i < method_count; ++i){  // 服务中有若干方法
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);    // 获取方法描述信息
        std::string method_name = pmethodDesc->name();    // 方法名称
        service_info.m_methodMap.insert({method_name, pmethodDesc});    // <方法名称, 方法描述>

        std::cout << "method name: " << method_name << std::endl;
    }
    service_info.m_service = service;
    m_servicesMap.insert({service_name, service_info});     // <服务名称, 服务信息>
}

void RpcProvider::Run()
{
    std::string ip = MprpcConfig::GetInstance()->GetConfigValue("rpcserverip");
    uint16_t port = stoi(MprpcConfig::GetInstance()->GetConfigValue("rpcserverport"));
    muduo::net::InetAddress m_addr(ip, port);

    muduo::net::TcpServer server(&m_eventloop, m_addr , "RpcProvider");

    // 绑定连接回调和消息读写回调
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 设置muduo的线程数量
    server.setThreadNum(4); // io + worker * 3

    ZkClient zkCli;
    zkCli.Start();

    // service永久节点，method临时节点
    for(auto &service : m_servicesMap)
    {
        std::string service_path = "/" + service.first;
        zkCli.Create(service_path.c_str(), nullptr, 0);
        for(auto &method : service.second.m_methodMap)
        {
            std::string method_path = service_path + "/" + method.first;
            char method_path_data[128] = {0};
            sprintf(method_path_data, "%s:%d", ip.c_str(), port);
            zkCli.Create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL);   // ZOO_EPHEMERAL 临时节点
        }
    }
    // 启动服务
    server.start();
    m_eventloop.loop();

}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if(!conn->connected()){
        conn->shutdown();
    }
}

/**
 * provider和consumer协商好通信要用的protobuf数据类型
 * service_name method_name args   应用层定义好这些message类型，序列化/反序列化数据
 */
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, 
    muduo::net::Buffer *buf, muduo::Timestamp time)
{
    std::string recv_buf = buf->retrieveAllAsString();  // 获取请求报文, 一个字符流
    
    // 反序列化
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4);
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    
    mprpc::RpcHeader rpcheader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size = 0;
    if(rpcheader.ParseFromString(rpc_header_str)){
        service_name = rpcheader.service_name();
        method_name = rpcheader.method_name();
        args_size = rpcheader.args_size();
    }else{
        std::cout << "rpc_header_str " << rpc_header_str << " parse error" << std::endl;
        return;
    }
    std::string args_str = recv_buf.substr(4 + header_size, args_size);     // 4=header_size
    std::cout << "============================================================" << std::endl;
    std::cout << "header_size" << header_size << " rpc_header_str: " << rpc_header_str << std::endl
              << "args_str" << args_str << std::endl << "service_name: "
              << service_name << std::endl << " method_name: " << method_name << std::endl;
    std::cout << "============================================================" << std::endl;

    // isexist
    auto it = m_servicesMap.find(service_name);
    if(it == m_servicesMap.end()){      // 服务
        std::cout << "service_name " << service_name << " not found" << std::endl;
        return;
    }
    auto mit = it->second.m_methodMap.find(method_name);
    if(mit == it->second.m_methodMap.end()){    // 方法
        std::cout << "method_name " << method_name << " not found" << std::endl;
        return;
    }

    google::protobuf::Service* service = it->second.m_service;      // service对象
    const google::protobuf::MethodDescriptor* method = mit->second; // method对象

    // 创建request和response的args
    google::protobuf::Message* request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str))
    {
        std::cout << "request args parse failed" << std::endl;
        return;
    }

    google::protobuf::Message* response = service->GetResponsePrototype(method).New();
 
    google::protobuf::Closure *done = google::protobuf::NewCallback
                                        <RpcProvider, const muduo::net::TcpConnectionPtr&, google::protobuf::Message*>
                                        (this, &RpcProvider::SendRpcResponse, conn, response);

    // 调用service的method
    // new Userservice().login(controller, request, response, done)
    service->CallMethod(method, nullptr, request, response, done);
}


void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response)
{
    std::string response_str;
    if(response->SerializeToString(&response_str))  // 序列化response
    {
        conn->send(response_str);
        conn->shutdown();
    }else{
        std::cout << "response serialize failed" << std::endl;
    }
    conn->shutdown();
}

}