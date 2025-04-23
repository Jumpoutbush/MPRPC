#pragma once
#include "const.h"
#include "lockqueue.h"

#define LOG_INFO(logmsgformat, ...) \
    do \
    {  \
        std::shared_ptr<Logger> logger = Logger::GetInstance(); \
        logger->SetLogLevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger->Log(c); \
    } while(0) \

#define LOG_ERR(logmsgformat, ...) \
    do \
    {  \
        std::shared_ptr<Logger> logger = Logger::GetInstance(); \
        logger->SetLogLevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger->Log(c); \
    } while(0) \

namespace mprpc{
// 定义日志级别
enum LogLevel
{
    INFO,  
    ERROR, 
};

class Logger : public Singleton<Logger>,
               public std::enable_shared_from_this<Logger>
{
    friend Singleton<Logger>;
public:
    // 设置日志级别 
    void SetLogLevel(LogLevel level);
    // 写日志
    void Log(std::string msg);
private:
    int m_loglevel; // 记录日志级别
    LockQueue<std::string>  m_lckQue; // 日志缓冲队列

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};

}