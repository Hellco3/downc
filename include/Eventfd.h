#ifndef _EVENTFD_H_
#define _EVENTFD_H_
#include <mutex>
#include <sys/eventfd.h>
#include <unistd.h>

class Eventfd
{
public:
    Eventfd();
    int fd()
    {
        return evtfd;
    }
    void Write(uint64_t value);
    uint64_t Read();
    ~Eventfd();

private:
    // std::mutex m_mtx;
    int evtfd;
};
#endif