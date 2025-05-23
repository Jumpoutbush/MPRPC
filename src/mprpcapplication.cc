#include "mprpcapplication.h"
// #include "rpcprovider.h"

#include <unistd.h>
#include <iostream>
namespace mprpc{


void ShowArgsHelp()
{
    std::cout << "<Usage> ./mprpcapp <rpc method> [args]" 
    << "-i <config file>" << std::endl
    << "-: get the help info" << std::endl
    << std::endl;
}
void MprpcApplication::Init(int argc,char **argv)
{
    if(argc < 2){   // 参数不足
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    while((c = getopt(argc, argv, "i:")) != -1)
    {
        switch(c)
        {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                std::cout << "invalid option: -" << (char)optopt << std::endl;
                ShowArgsHelp();
                exit(EXIT_FAILURE);
                break;
            case ':':
                ShowArgsHelp();
                exit(EXIT_FAILURE);
                break;
            default:
                break;
        }
    }

    /**
     * 初始化配置文件
     * rpcserver_ip, rpcserver_port, zookeeper_ip, zookeeper_port
     */ 
    MprpcConfig::GetInstance()->LoadConfigFile(config_file.c_str());
    std::cout << "rpcserver ip: " << MprpcConfig::GetInstance()->GetConfigValue("rpcserverip") << std::endl;
    std::cout << "rpcserver port: " << MprpcConfig::GetInstance()->GetConfigValue("rpcserverport") << std::endl;
    std::cout << "zookeeper ip: " << MprpcConfig::GetInstance()->GetConfigValue("zookeeperip") << std::endl;
    std::cout << "zookeeper port: " << MprpcConfig::GetInstance()->GetConfigValue("zookeeperport") << std::endl;
}

// MprpcConfig& MprpcApplication::GetConfig()
// {
//     return m_config;
// }

}