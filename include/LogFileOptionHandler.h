#ifndef __LOGFILE_OPTION_HANDLER__
#define __LOGFILE_OPTION_HANDLER__
#include <string>

#include "OptionHandler.h"
#include "Logger.h"

// 日志文件路径设置选项类
class LogFileOptionHandler
    : public OptionHandler
{
public:
    LogFileOptionHandler(void *arg);
    void doHandle();

private:
    std::string _logpath;
};
#endif
