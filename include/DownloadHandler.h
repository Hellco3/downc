#ifndef __DOWNLOAD_HANDLER__
#define __DOWNLOAD_HANDLER__
#include <string>
#include <map>
#include <memory>

#include "HttpConn.h"
#include "HttpsConn.h"

// 下载类，实现对各协议栈细节的隐藏，只暴露出getData接口用于获取数据到缓冲区中
class DownloadHandler {
public:
    DownloadHandler();
    void Init();
    size_t getFileSize(const std::string & uri);
    std::shared_ptr<Conn> getConn(const std::string & protocol);
    int getFinalUrl(std::string & url);
    
    int getData(const std::string &uri, char* buf,size_t offset, size_t size);
    ~DownloadHandler();
private:
    std::map<std::string, std::shared_ptr<Conn>> _conn_map;
};

#endif