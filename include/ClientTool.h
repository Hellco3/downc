#ifndef Client_Tool_H
#define Client_Tool_H

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
     * @param poolname 存储池的名字
     * @param object_id 文件id
     * @param bRandom 是否开启随机写
    */
    Client_Tool(int argc, char ** argv, int threadNum=10, int queSize=20);
    ~Client_Tool();
    
    int OptionProcess(std::vector<std::string>& uris, int argc, char** argv);
    
    int start();

    void setPrefix(const string & pre);
    /**
     * @brief 单次下载任务
     * @param object_id 文件id
     * @param bRandom 是否开启随机读 
    */

protected:
    int getFileFd(const std::string & filename, FileOp* fileop=NULL);

    void removeFileFd(const std::string & filename);

    int DownloadForUrl(std::string& uri);

    void DownloadTask(std::string& uri, size_t offset, size_t size, size_t file_size);

    void add_to_output(const string& str);

    void pop_outstr(ostream & os);
private:
    unsigned long getCurMicSec();
    void mhelp(const char *project_name);
private:
    MutexLock m_lock;
    MutexLock file_lock;
    ThreadPool _clientpool;
    
    std::string out_str;
    std::string prefix="";
    std::unique_ptr<DownloadHandler> _meta_handler;

    vector<std::shared_ptr<OptionHandler>> _ohandlerlist;
    vector<std::shared_ptr<DownloadHandler>> _dhandlerlist;

    std::map<std::string, FileOp> _file_map;

    std::vector<char*> _buflist;
    vector<std::string> _uris;
    bool _isExit = false;
};

using std::ifstream;
using std::ofstream;
/**
 * @brief 生成随机字符串
 * @param length 指定随机字符串的长度
 * @param genstring 存放随机字符串的char指针
 * @return 生成的随机字符串的首地址
*/
char * genRandomString(int length, char *genstring);

/**
 * @brief 生成随机数组
 * @param max 设置数组中的最大值
 * @param length 设置数组的长度
 * @param objectvec 接收随机数的数组
*/
void genRandomVec(uint64_t max, int length, vector<uint64_t> &objectvec);
/**
 * @brief 获取文件大小
 * @param filepath 文件路径 
 * @return 返回文件大小
*/
uint64_t get_file_size(const string & filepath);
/// @brief 用于初始化测试文件
void init_files();

/**
 * @brief 创造一个指定大小的文件
 * @param filepath 存放该文件的文件路径
 * @param file_size 指定文件的大小
 * @return 成功返回0，失败返回-1
*/
int create_file(const string & filepath, uint64_t file_size);

/**
 * @brief 拷贝文件
 * @param oldfile 拷贝源文件的路径 
 * @param newfile 将要保存新文件的路径
 * @return 成功返回0，失败返回-1
*/
int copy_file(const string & oldfile, const string &newfile);

/**
 * @brief 在指定范围更新文件
 * @param filepath 文件路径 
 * @param offset 文件偏移值
 * @param size 更新的长度
 * @return 成功返回更新成功的长度
*/
uint64_t update_file(const string & filepath, uint64_t offset, uint64_t size);

/**
 * @brief 追加写
 * @param filepath 
 * @param file_size 
 * @return 成功返回追加写的长度，失败返回-1
*/
int addwrite_file(const string & filepath, uint64_t size);

int f_addwrite(uint64_t object_id, uint64_t size);
int f_update(uint64_t object_id, uint64_t offset, uint64_t size);
int f_upload_file(uint64_t object_id, const char * filename);
int f_upload_list_task(int n, uint64_t start_object_id, uint64_t object_count, const char * filepath=NULL);
int f_delete_list(uint64_t object_id, uint64_t object_count);
int f_update_list(int n, uint64_t object_id, uint64_t object_count, uint64_t offset, uint64_t size);
int f_multiupdate(uint64_t object_id, char ** arg);
int f_multiaddwrite(uint64_t object_id, char ** arg);
using std::string;
#endif