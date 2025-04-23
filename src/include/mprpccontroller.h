#pragma once

#include "const.h"
#include "mprpccontroller.h"

#include <google/protobuf/service.h>

namespace mprpc{
/**
 * @brief 携带一些RPC调用的上下文信息，知道当前调用状态
 */ 
class MprpcController : public google::protobuf::RpcController{
public:
    MprpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void StartCancel();
    void SetFailed(const std::string& reason);
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);
private:
    bool m_failed;
    std::string m_errText;
};


}