#include "mprpccontroller.h"

namespace mprpc 
{

MprpcController::MprpcController()
{
    m_failed = false;
    m_errText = "";
}
void MprpcController::Reset()
{
    m_failed = false;
    m_errText = "";
}
bool MprpcController::Failed() const
{
    return m_failed;
}
std::string MprpcController::ErrorText() const
{
    return m_errText;
}
void MprpcController::StartCancel()
{
    // todo
}
void MprpcController::SetFailed(const std::string& reason)
{
    m_failed = true;
    m_errText = reason;
}
bool MprpcController::IsCanceled() const
{
    return m_failed;
}
void MprpcController::NotifyOnCancel(google::protobuf::Closure* callback)
{
    // todo
}
}