#include <string.h>

#include "Logger.h"
#include "LogLevelOptionHandler.h"

LogLevelOptionHandler::LogLevelOptionHandler(void *arg)
{
    _level = static_cast<char *>(arg);
}

void LogLevelOptionHandler::doHandle()
{
    if (!strcasecmp(_level.c_str(), "info"))
    {
        Mylogger::getInstance()->setLogLevel(log4cpp::Priority::INFO);
    }
    else if (!strcasecmp(_level.c_str(), "debug"))
    {
        Mylogger::getInstance()->setLogLevel(log4cpp::Priority::DEBUG);
    }
    else if (!strcasecmp(_level.c_str(), "warn"))
    {
        Mylogger::getInstance()->setLogLevel(log4cpp::Priority::WARN);
    }
    else if (!strcasecmp(_level.c_str(), "error"))
    {
        Mylogger::getInstance()->setLogLevel(log4cpp::Priority::ERROR);
    }
}