
#include "ClientTool.h"
#include "FileUnit.h"
#include "LogFileOptionHandler.h"
#include "LogLevelOptionHandler.h"
#include "PrefixOptionHandler.h"
#include "uri_split.h"
#include "Uri.h"
#include "ProtocolDetector.h"

#define BUF_SIZE 2*1024*1024
#define OPTSTRING ":l:hL:p:"

__thread int handlerid = 0;
Client_Tool * g_tool;

Client_Tool::Client_Tool(int argc, char** argv, int threadNum, int queSize)
: _clientpool(threadNum, queSize)
{
    if(-1 == OptionProcess(_uris, argc, argv))
    {
        _isExit = true;       
    }
    else {
        curl_global_init(CURL_GLOBAL_ALL);
        for(int idx=0; idx<threadNum; idx++)
        {
            _dhandlerlist.push_back(std::shared_ptr<DownloadHandler>(new DownloadHandler));
            char* ptr = new char[BUF_SIZE];
            _BufList.push_back(ptr);
        }
        _meta_handler.reset(new DownloadHandler);
    }
}

Client_Tool::~Client_Tool()
{
    for(auto iter : _BufList)
    {
        delete []iter;
    }
    _clientpool.stop();
    curl_global_cleanup();
}

int Client_Tool::OptionProcess(std::vector<std::string>& uris, int argc, char** argv)
{
    int opt;
    int option_index = 0;
    struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"log", required_argument, 0, 'l'},
        {"console-log-level", required_argument, 0, 'L'},
        {"prefix", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };
    while(true)
    {
        if((opt=getopt_long(argc, argv, OPTSTRING, long_options, &option_index))!=-1)
        {
            switch(opt) {
                case 'h':
                {
                    mhelp(argv[0]);
                    return -1;
                    break;
                }
                case 'l':
                {
                    std::shared_ptr<OptionHandler> op(new LogFileOptionHandler(optarg));
                    _ohandlerlist.push_back(op);
                    break;
                }
                case 'L':
                {
                    std::shared_ptr<OptionHandler> op(new LogLevelOptionHandler(optarg));   
                    _ohandlerlist.push_back(op);
                    break;
                }
                case 'p':
                {
                    std::shared_ptr<OptionHandler> op(new PrefixOptionHandler(optarg));
                    _ohandlerlist.push_back(op);
                    break;
                }
                case '?':
                {
                    logError("unknown option: %c", opt);
                    return -1;
                    break;
                }
                case ':':
                {
                    logError("option:%c requires an argument", opt);
                    return -1;
                    break;
                }
            }
        } else 
            break;
    }
    std::copy(argv + optind, argv+argc, std::back_inserter(uris));
    return 0;
}

int Client_Tool::start()
{
    if(!_isExit)
    {
        g_tool = this;
        for(auto oiter : _ohandlerlist)
        {
            oiter->doHandle();
        }

        _clientpool.start();

        for(auto &uiter : _uris)
        {
            logDebug("downloadForUrl url:%s", uiter.c_str());
            if(0 != DownloadForUrl(uiter))
            {
                logError("Download url:%s error", uiter.c_str());
            }
        }
        pop_outstr(cout);
    }
    return 0;
}
    
void Client_Tool::setPrefix(const string & pre)
{
    prefix = pre;
}

int Client_Tool::getFileFd(const std::string & filename, FileOp* fileop)
{
    MutexLockGuard mutexGuard(file_lock);
    auto iter = _file_map.find(filename);
    if(iter == _file_map.end())
    {
        int file_fd = open(filename.c_str(), O_WRONLY|O_CREAT);
        if(-1 == file_fd)
        {
            logError("path:%s invalid", filename.c_str());
            return -1;
        }
        struct FileOp fileop;
        fileop.filefd = file_fd;
        fileop.mtx = new MutexLock;
        fileop.processbar = new ProcessBar;
        _file_map[filename] = fileop;
    }
    if(fileop != NULL)
    {
        *fileop = _file_map[filename];
    }
    return _file_map[filename].filefd;
}

void Client_Tool::removeFileFd(const std::string & filename)
{
    MutexLockGuard mutexGuard(file_lock);
    auto iter = _file_map.find(filename);
    if(iter != _file_map.end())
    {
        delete iter->second.mtx;
        delete iter->second.processbar;
        close(iter->second.filefd);
        _file_map.erase(iter);
    }
}

// 先通过获取报文头来获取文件的大小，在将文件按2M为单位进行分发，文件小于2M的会由单线程完成下载，大于2M的以2M为单位给各线程分发任务
int Client_Tool::DownloadForUrl(std::string uri)
{
    size_t file_size = _meta_handler->getFileSize(uri);
    if(-1 == (int)file_size)
    {
        logError("getFileSize failed! uri:%s", uri.c_str());
        return -1;
    }
    logDebug("file_size: %lu", file_size);
    UriStruct uStruct;
    getUriStruct(uri,uStruct);
    struct FileOp fileop;
    std::string filepath = prefix+uStruct.file;
    int file_fd = getFileFd(filepath, &fileop);
    if(-1 == file_fd)
    {
        logError("getFileFd error!! filepath:%s", filepath.c_str());
        return -1;
    }
    fileop.processbar->start(file_size, uStruct.file);
    size_t offset = get_file_size(prefix+uStruct.file);
    while(offset < file_size)
    {
        int nread = BUF_SIZE;
        if(file_size - offset < BUF_SIZE)
            nread = file_size - offset;
        _clientpool.addTask(std::bind(&Client_Tool::DownloadTask, this, uri, offset, nread, file_size));
        offset += nread;
    }
    return 0;
}

void Client_Tool::add_to_output(const string& str)
{
    MutexLockGuard locker(m_lock);
    out_str += str;
}

void Client_Tool::pop_outstr(ostream & os)
{
    os << out_str;
}

void Client_Tool::mhelp(const char *project_name)
{
    printf("-------------------------------------\n");
    printf("please in put param for example: \n");
    printf("demo:\n");
    printf("./%s https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ls-lR.gz", project_name);
    printf("./%s --log <logfile> 设置日志文件保存位置", project_name);
    printf("./%s --console-log-level <log-level> 设置日志登记:error/warn/info/debug", project_name);
    printf("./%s --prefix <filepath> 设置文件保存路径", project_name);
    printf("--------------------------------------\n");
}

// 分发给线程池的单个下载任务函数
void Client_Tool::DownloadTask(std::string & uri, size_t offset, size_t size, size_t file_size)
{
    logDebug("uri:%s offset:%ld size:%ld file_size:%lu", uri.c_str(), offset, size, file_size);
    UriStruct uStruct;
    getUriStruct(uri, uStruct);
    std::string filepath = prefix+uStruct.file;

    FileOp fileop;
    int file_fd = getFileFd(filepath, &fileop);
    if(-1 == file_fd)
    {
        logError("getFileFd failed. uri:%s", uri.c_str());
        return;
    }
    char * getbuf = _BufList[handlerid];
    bzero(getbuf, BUF_SIZE);
    std::shared_ptr<DownloadHandler>pHandler = _dhandlerlist[handlerid];
    int ret = pHandler->getData(uri, getbuf, offset, size);
    if (0 != ret)
    {
        logError("getData error. uri:%s offset:%ld size:%ld", uri.c_str(), offset, size);
        return;
    }
    pthread_mutex_lock(fileop.mtx->getMutexLockPtr());
    lseek(file_fd, offset, SEEK_SET);
    ret = write(file_fd, getbuf, size);
    if(ret < 0)
    {
        logError("write error! filepath: %s",filepath);
    } 
    fileop.processbar->add(size);
    pthread_mutex_unlock(fileop.mtx->getMutexLockPtr());
    if(offset+size >= file_size)
    {
        removeFileFd(filepath);
    }
}
    