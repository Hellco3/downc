#include <string>

#include "OptionHandler.h"

// 文件保存路径设置选项类
class PrefixOptionHandler
    : public OptionHandler
{
public:
    PrefixOptionHandler(void *arg);
    void doHandle();

private:
    std::string _prefix;
};