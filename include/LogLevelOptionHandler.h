#ifndef __LOGLEVEL_OPTION_HANDLER__
#define __LOGLEVEL_OPTION_HANdlEr__
#include "OptionHandler.h"
#include <string>

class LogLevelOptionHandler
: public OptionHandler
{
public:
    LogLevelOptionHandler(void * arg);
    void doHandle();
private:
    std::string _level;
};
#endif