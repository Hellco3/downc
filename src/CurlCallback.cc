#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "CurlCallback.h"
#include "Logger.h"

size_t redirectCallback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    // 将目标URL存储在userdata中
    memcpy(userdata, buffer, nitems * size);
    return nitems * size;
}

size_t writeData(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb; // 本次响应数据的实际大小
    char *mem = (char *)userp;      // 获取用户传进的变量

    memcpy(mem, data, realsize);

    return realsize;
}