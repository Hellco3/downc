#ifndef __HTTPS_CONN_H__
#define __HTTPS_CONN_H__
#include <string.h>

#include <string>
#include <curl/curl.h>

#include "Conn.h"

// Https连接类
class HttpsConn
: public Conn
{
public:
    HttpsConn();
    ~HttpsConn();
    size_t getSizeForUrl(const std::string & uri);
    int getFinalUrl(std::string &uri);
    void init();
    int execute(void * arg);
protected:
    CURL*  m_curl;
};
#endif