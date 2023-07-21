#include <string.h>
#include <string>

#include "Conn.h"

Protocol ProtocolToEnum(const std::string &protocol)
{
    if (!strcasecmp(protocol.c_str(), "http"))
    {
        return HTTP;
    }
    else if (!strcasecmp(protocol.c_str(), "https"))
    {
        return HTTPS;
    }
    else if (!strcasecmp(protocol.c_str(), "ftp"))
    {
        return FTP;
    }
    else
    {
        return PROTO_DEFAULT;
    }
}