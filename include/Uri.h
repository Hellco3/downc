#ifndef _D_URI_H
#define _D_URI_H
#include <string>
#include "UriSplit.h"

struct UriStruct
{
    std::string protocol;
    std::string host;
    std::string dir;
    std::string file;
    std::string query;
    std::string username;
    std::string password;
    uint16_t port;
    bool hasPassword;
    bool ipv6LiteralAddress;

    UriStruct();
    UriStruct(const UriStruct &c);
    ~UriStruct();

    UriStruct &operator=(const UriStruct &c);
    void swap(UriStruct &other);
};

int getUriStruct(const std::string& uri, UriStruct & uri_s);
#endif