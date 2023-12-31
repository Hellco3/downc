#ifndef __HTTP_CONN__
#define __HTTP_CONN__
#include <string.h>

#include <string>
#include <curl/curl.h>

#include "Conn.h"

//Http连接类
class HttpConn
: public Conn
{
public:
    HttpConn();
    ~HttpConn();
    size_t getSizeForUrl(const std::string & uri);
    int getFinalUrl(std::string &uri);
    void init();
    int execute(void * arg);
protected:
    CURL*  m_curl;
};

#endif