#include "HttpConn.h"
#include "Logger.h"

HttpConn::HttpConn()
{
    m_curl = curl_easy_init();
    if (m_curl == NULL)
    {
        logError("curl init failed!");
    }
}

HttpConn::~HttpConn()
{
    curl_easy_cleanup(m_curl);
}

void HttpConn::init()
{
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L); // 启用重定向跟随
}

size_t HttpConn::getSizeForUrl(const std::string &uri)
{
    if (m_curl)
    {
        size_t downloadFileLength = 0;
        curl_easy_setopt(m_curl, CURLOPT_URL, uri.c_str());
        curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(m_curl, CURLOPT_NOBODY, 1);
#ifdef CURL_DEBUG
        curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);
#endif
        if (curl_easy_perform(m_curl) == CURLE_OK)
        {
            curl_easy_getinfo(m_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLength);
        }
        else
        {
            downloadFileLength = -1;
        }
        curl_easy_reset(m_curl);
        init();
        return downloadFileLength;
    }
    else
    {
        logError("m_curl is null");
        return -1;
    }
}

int HttpConn::getFinalUrl(std::string &uri)
{
    if (m_curl)
    {
        char redirectURL[256] = {0}; // 存储重定向的目标URL

        curl_easy_setopt(m_curl, CURLOPT_URL, uri.c_str());
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L); // 启用重定向跟随
        curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, redirectCallback);
        curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, redirectURL);
        curl_easy_setopt(m_curl, CURLOPT_NOBODY, 1);

        CURLcode res;
        res = curl_easy_perform(m_curl);

        if (res == CURLE_OK)
        {
            logDebug("Redirected URL: %s\n", redirectURL);
            curl_easy_reset(m_curl);
            uri = redirectURL;
            return 0;
        }
        else
        {
            logError("curl_easy_perform() failed: %s", curl_easy_strerror(res));
            curl_easy_reset(m_curl);
            return -1;
        }
    }
    else
    {
        logError("m_curl is null");
        return -1;
    }
}

int HttpConn::execute(void *arg)
{
    DownMsg *downMsg = static_cast<DownMsg *>(arg);
    char *getbuf = downMsg->buf;
    CURLcode res;
    char range[256] = {0};
    sprintf(range, "%lu-%lu", downMsg->offset, downMsg->offset + downMsg->size);
    if (m_curl)
    {
        curl_easy_setopt(m_curl, CURLOPT_URL, downMsg->uri.c_str());
        curl_easy_setopt(m_curl, CURLOPT_RANGE, range);
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
        logDebug("transfer uri:%s, range:%s", downMsg->uri, range);
        if (getbuf)
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, getbuf);
        res = curl_easy_perform(m_curl);
        if (res != CURLE_OK)
        {
            logError("curl_easy_perform() failed: %s", curl_easy_strerror(res));
            return -1;
        }
        return 0;
    }
    else
    {
        logError("m_curl is null");
        return -1;
    }
}