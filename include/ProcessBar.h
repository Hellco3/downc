#ifndef _PROCESSBAR_H_
#define _PROCESSBAR_H_
#include <iostream>
#include <string>
#include <iomanip>
#include <pthread.h>
#include "Eventfd.h"

// 一个封装好的进度条类
class ProcessBar
{
public:
    ProcessBar();
    void start(uint64_t size, const std::string &file_name);
    void add(uint64_t num);
    void stop();
    static void *thread_func(void *data);
    ~ProcessBar();

private:
    pthread_t pid;
    uint64_t done_size;
    uint64_t total_size;
    Eventfd evtfd;
    std::string _file_name;
    bool running;
};

#endif