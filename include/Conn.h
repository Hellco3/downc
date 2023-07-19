#ifndef __CURL_CONN__
#define __CURL_CONN__
#include <memory>

#include "CurlCallback.h"

enum Protocol {
    HTTP,
    HTTPS,
    FTP,
    MAGNET,
    TORRENT,
    PROTO_DEFAULT
};

struct DownMsg
{
    std::string uri;
    size_t offset;
    size_t size;
    char * buf;
};

class Conn{
public:
    Conn()=default;
    ~Conn()=default;
    virtual size_t getSizeForUrl(const std::string & uri){return 0;}
    virtual int getFinalUrl(std::string &uri){return 0;}
    virtual void Init()=0;
    virtual int Execute(void *)=0;
};

Protocol ProtocolToEnum(const std::string & protocol);
#endif