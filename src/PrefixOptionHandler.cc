#include "PrefixOptionHandler.h"
#include "ClientTool.h"

PrefixOptionHandler::PrefixOptionHandler(void *arg)
{
    _prefix = static_cast<char *>(arg);
}

void PrefixOptionHandler::doHandle()
{
    g_tool->setPrefix(_prefix);
}