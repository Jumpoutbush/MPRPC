#pragma once
#include "const.h"
#include "mprpcconfig.h"

namespace mprpc
{
// mprpc框架的基础类，负责框架的一些初始化操作，使用单例模式设计
class MprpcApplication : public Singleton<MprpcApplication>, 
    public std::enable_shared_from_this<MprpcApplication>
{
    friend class Singleton<MprpcApplication>;
public:
    ~MprpcApplication();
    static void Init(int argc,char **argv);
    static MprpcConfig& GetConfig();
private:
    MprpcApplication();
    MprpcApplication(const MprpcApplication&)=delete;
    MprpcApplication(MprpcApplication&&)=delete;

    static MprpcConfig m_config;
};

}