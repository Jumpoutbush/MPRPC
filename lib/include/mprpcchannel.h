#pragma once 
#include "const.h"
#include "const.h"
#include "mprpcconfig.h"
#include "zookeeperutil.h"

#include <arpa/inet.h>
#include <errno.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace google::protobuf;
namespace mprpc{
class MprpcChannel : public google::protobuf::RpcChannel 
{
public:
    MprpcChannel(){};
    void CallMethod(const MethodDescriptor* method,
                    RpcController* controller, const Message* request,
                    Message* response, Closure* done);
private:

};

}