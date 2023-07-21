#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Task.h"
#include "Thread.h"
#include "TaskQueue.h"
#include <vector>
#include <memory>

using std::unique_ptr;
using std::vector;

// 一个封装好的线程池
class ThreadPool
{
public:
    ThreadPool();
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    // 启动函数
    void start();
    // 关闭函数，会等待所有线程的任务完成后再关闭
    void stop();

    // 向线程池中插入一个任务，各空闲线程会抢夺该任务
    void addTask(Task &&cb);
    Task getTask();

    void setargs(size_t threadNum, size_t queSize);

private:
    void threadFunc();

private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;
};

#endif