#ifndef __Client_Tool_H__
#define __Client_Tool_H__

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <getopt.h>

#include <memory>
#include <unordered_map>
#include <vector>
#include <thread>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>

#include "Logger.h"
#include "ThreadPool.h"
#include "ProcessBar.h"
#include "OptionHandler.h"
#include "DownloadHandler.h"

#define BUF_SIZE 2*1024*1024

using namespace std::chrono;
using namespace std;

struct FileOp {
    int filefd;
    MutexLock * mtx;
    ProcessBar * processbar;
};

class Client_Tool
{
public:
    /**
     * @brief Client_Tool的构造函数
    */
    Client_Tool(int argc, char ** argv, int threadNum=10, int queSize=20);
    ~Client_Tool();
    
    // 命令行解析函数
    int OptionProcess(std::vector<std::string>& uris, int argc, char** argv);
    
    int start();

    // 设置文件保存路径
    void setPrefix(const string & pre);

protected:
    int getFileFd(const std::string & filename, FileOp* fileop=NULL);

    void removeFileFd(const std::string & filename);

    int downloadForUrl(std::string uri);

    void downloadTask(std::string& uri, size_t offset, size_t size, size_t file_size);

    void addToOutput(const string& str);

    void popOutput(ostream & os);

private:
    void mhelp(const char *project_name);
private:
    MutexLock m_lock;
    MutexLock file_lock;
    // 线程池
    ThreadPool _clientpool;
    
    std::string out_str;
    std::string prefix="";
    // 用于元数据获取的句柄
    std::unique_ptr<DownloadHandler> _meta_handler;

    // 选项操作类列表
    vector<std::shared_ptr<OptionHandler>> _ohandlerlist;
    // 下载操作类列表
    vector<std::shared_ptr<DownloadHandler>> _dhandlerlist;

    // 文件名与文件操作句柄的映射表
    std::map<std::string, FileOp> _file_map;

    std::vector<char*> _BufList;
    vector<std::string> _uris;
    bool _isExit = false;
};

uint64_t getFileSize(const string & filepath);

#endif