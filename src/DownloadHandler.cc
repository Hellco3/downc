#include "DownloadHandler.h"
#include "Uri.h"
#include "Logger.h"

DownloadHandler::DownloadHandler()
{
}

void DownloadHandler::Init()
{
}

std::shared_ptr<Conn> DownloadHandler::getConn(const std::string &protocol)
{
    auto iter = _conn_map.find(protocol);
    if (iter == _conn_map.end())
    {
        switch (ProtocolToEnum(protocol))
        {
        case HTTP:
        {
            std::shared_ptr<Conn> pConn(new HttpConn);
            _conn_map[protocol] = pConn;
            break;
        }
        case HTTPS:
        {
            std::shared_ptr<Conn> pConn(new HttpsConn);
            _conn_map[protocol] = pConn;
            break;
        }
        default:
        {
            logError("Protocol:%s invalid", protocol.c_str());
            break;
        }
        }
    }
    return _conn_map[protocol];
}

size_t DownloadHandler::getFileSize(const std::string &uri)
{
    UriStruct uStruct;
    getUriStruct(uri, uStruct);
    std::shared_ptr<Conn> pConn = getConn(uStruct.protocol);
    return pConn->getSizeForUrl(uri);
}

int DownloadHandler::getFinalUrl(std::string &url)
{
    UriStruct uStruct;
    getUriStruct(url, uStruct);
    std::shared_ptr<Conn> pConn = getConn(uStruct.protocol);
    return pConn->getFinalUrl(url);
}

// 解析URL，不同的协议的链接会分配给不同的协议连接类进行远程数据获取，最终数据会写入到buf内存缓冲区中
int DownloadHandler::getData(const std::string &uri, char *buf, size_t offset, size_t size)
{
    DownMsg dMsg;
    dMsg.uri = uri;
    dMsg.offset = offset;
    dMsg.size = size;
    dMsg.buf = buf;
    UriStruct uStruct;
    getUriStruct(uri, uStruct);
    return _conn_map[uStruct.protocol]->execute(&dMsg);
}

DownloadHandler::~DownloadHandler()
{
}