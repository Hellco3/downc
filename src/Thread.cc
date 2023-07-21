#include "Thread.h"

extern __thread int handlerid;

Thread::Thread(ThreadCallback &&cb, int id)
    : _thid(0), _isRunning(true), _cb(std::move(cb)), _thread_id(id)
{
}

Thread::~Thread()
{
    if (_isRunning)
    {
        pthread_detach(_thid);
        _isRunning = false;
    }
}

void Thread::start()
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if (ret)
    {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void Thread::join()
{
    if (_isRunning)
    {
        pthread_join(_thid, nullptr);
        _isRunning = false;
    }
}

void *Thread::threadFunc(void *arg)
{
    Thread *pth = static_cast<Thread *>(arg);
    handlerid = pth->_thread_id;
    if (pth)
    {
        pth->_cb();
    }
    return nullptr;
}