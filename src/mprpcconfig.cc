#include "mprpcconfig.h"

namespace mprpc {

MprpcConfig::MprpcConfig()
{

}

MprpcConfig::~MprpcConfig()
{

}
void MprpcConfig::LoadConfigFile(const char* filename)
{
    FILE *pf = fopen(filename, "r"); //以只读方式打开文件
    if (pf == nullptr){
        std::cout << filename << " config_file open failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    // 处理：
    // 注释符：#
    // 正确的配置项 ：key = value
    // 去掉开头多余的空格
    while(!feof(pf)){
        char buf[1024] = {0};
        fgets(buf, 1024, pf);   // 读取一行
        // 去掉开头多余的空格
        std::string src_buf(buf);
        Trim(src_buf);
        // 判断是否为注释
        if(src_buf[0] == '#' || src_buf.empty()){
            continue; // 跳过这行
        }
        // 判断是否为配置项
        int idx = src_buf.find('=');
        if(idx == std::string::npos){
            continue; // 跳过这行
        }
        std::string key = src_buf.substr(0, idx);
        Trim(key);
        std::string value = src_buf.substr(idx + 1, src_buf.size() - idx);
        Trim(value);
        // 去掉value两边的空格
        idx = value.find_first_not_of(" \t\r\n") && value.find_first_not_of(' ');
        if(idx != std::string::npos){
            value = value.substr(idx, value.size() - idx);
        }
        idx = value.find_last_not_of(" \t\r\n") && value.find_first_not_of(' ');
        if(idx != std::string::npos){
            value = value.substr(idx, value.size() - idx);
        }
        // 将配置项存入map
        m_configMap.insert(std::make_pair(key, value));
    }
}

std::string MprpcConfig::GetConfigValue(const char* key)
{
    auto it = m_configMap.find(key);
    if(it == m_configMap.end()){
        return "";
    }
    return it->second;
}

void MprpcConfig::Trim(std::string& src_buf)
{
    if(src_buf.empty()){
        return;
    }
    // 去掉前面多余的空格
    int idx = src_buf.find_first_not_of("\t\r\n' '");
    if(idx != std::string::npos){
        src_buf = src_buf.substr(idx, src_buf.size() - idx);
    }
    // 去掉后面多余的空格
    idx = src_buf.find_last_not_of("\t\r\n' '");
    if(idx != std::string::npos){
        src_buf = src_buf.substr(0, idx + 1);
    }
}
}