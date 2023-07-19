#ifndef __HTTP_CONN__
#define __HTTP_CONN__
#include <string.h>

#include <string>
#include <curl/curl.h>

#include "Conn.h"

class HttpConn
: public Conn
{
public:
    HttpConn();
    ~HttpConn();
    size_t getSizeForUrl(const std::string & uri);
    int getFinalUrl(std::string &uri);
    void Init();
    int Execute(void * arg);
protected:
    CURL*  m_curl;
};

#endif