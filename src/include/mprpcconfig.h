#pragma once
#include <unordered_map>
#include "const.h"
/**
 *  @brief 框架读取配置文件类 
 */
namespace mprpc{
class MprpcConfig
{
public:
    MprpcConfig();
    ~MprpcConfig();

    /**
     * @brief 读取配置文件
     * 
     * @param filename 配置文件路径
     */
    void LoadConfigFile(const char* filename);

    /**
     * @brief 获取配置文件中的值
     * 
     * @param key 配置项名称
     * @return std::string 配置项的值
     */
    std::string GetConfigValue(const char* key);

    /**
     * @brief 去除字符串首尾空格
     */
    void Trim(std::string& str);

private:
    std::unordered_map<std::string, std::string> m_configMap;
};

}