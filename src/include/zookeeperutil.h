#pragma once
#include "const.h"
#include "mprpcconfig.h"

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

namespace mprpc
{
/**
 * @brief zk客户端
 */
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    // 启动连接zk
    void Start();
    // 创建znode节点
    void Create(const char *path, const char *data, int datalen, int state=0);
    
    std::string GetData(const char *path);
private:
    // zk的客户端句柄
    zhandle_t *m_zhandle;
};

}