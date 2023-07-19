#include "LogFileOptionHandler.h"

LogFileOptionHandler::LogFileOptionHandler(void * arg)
{
    _logpath = static_cast<char*>(arg);
}

void LogFileOptionHandler::doHandle()
{
    Mylogger::getInstance()->setLogFile(_logpath.c_str());
}