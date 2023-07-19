#include <string>

#include "OptionHandler.h"

class PrefixOptionHandler
: public OptionHandler
{
public:
    PrefixOptionHandler(void * arg);
    void doHandle();
private:
   std::string _prefix;
};