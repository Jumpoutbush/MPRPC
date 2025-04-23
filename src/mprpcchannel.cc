#include "mprpcchannel.h"

using namespace google::protobuf;

namespace mprpc
{
/**
 * header_size+service_name method_name args_size+args
 */
void MprpcChannel::CallMethod(const MethodDescriptor* method,
    RpcController* controller, const Message* request,
    Message* response, Closure* done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    int args_size = 0;
    std::string args_str;
    if(request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }else{
        std::cout << "serialize request failed" << std::endl;
        controller->SetFailed("serialize request failed");
        return;
    }

    // request header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    std::string rpc_header_str;
    uint32_t header_size = 0;
    if(rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }else{
        std::cout << "serialize rpc header failed" << std::endl;
        controller->SetFailed("serialize rpc header failed");
        return;
    }

    // 组织待发送的rpc请求的字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4));    // 4 = header_size
    send_rpc_str += rpc_header_str + args_str;

    std::cout << "============================================================" << std::endl;
    std::cout << "header_size" << header_size << " rpc_header_str: " << rpc_header_str << std::endl
              << "args_str" << args_str << std::endl << "service_name: " << service_name << std::endl
              << " method_name: " << method_name << std::endl << "args_str: " << args_str << std::endl;
    std::cout << "============================================================" << std::endl;

    // tcp远程调用
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd == -1){
        char errtext[512] = {0};
        sprintf(errtext, "create clientfd failed:%d", errno);
        controller->SetFailed(errtext);
        exit(EXIT_FAILURE);
    }

    ZkClient zkCli;
    zkCli.Start();
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string host_data = zkCli.GetData(method_path.c_str());
    if(host_data == ""){
        controller->SetFailed(method_path + " is not exist");
        return;
    }
    int idx = host_data.find(":");
    if(idx == -1)
    {
        controller->SetFailed(method_path + " address is invalid");
        return;
    }
    
    std::string ip = MprpcConfig::GetInstance()->GetConfigValue("rpcserverip");
    uint16_t port = stoi(MprpcConfig::GetInstance()->GetConfigValue("rpcserverport"));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 连接rpc服务节点
    if(connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1)
    {
        char errtext[512] = {0};
        sprintf(errtext, "connect failed:%d", errno);
        controller->SetFailed(errtext);
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    // send rpc request
    if(send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0) == -1)
    {
        char errtext[512] = {0};
        sprintf(errtext, "send failed:%d", errno);
        controller->SetFailed(errtext);
        close(clientfd);
        return;
    }

    // recv rpc response
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if((recv_size = recv(clientfd, recv_buf, 1024, 0)) == -1)
    {
        char errtext[512] = {0};
        sprintf(errtext, "recv failed:%d", errno);
        controller->SetFailed(errtext);
        close(clientfd);
        return;
    }

    // write response
    // std::string response_str(recv_buf, 0, recv_size);
    if(!response->ParseFromArray(recv_buf, recv_size))
    {
        char errtext[512] = {0};
        sprintf(errtext, "parse response failed:%d", errno);
        controller->SetFailed(errtext);
        close(clientfd);
        return;
    }
    close(clientfd);
}

}