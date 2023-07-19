#ifndef __OPTIONHANDLER__
#define __OPTIONHANDLER__
class Client_Tool;
extern Client_Tool* g_tool;
class OptionHandler
{
public:
    virtual void doHandle();
};
#endif