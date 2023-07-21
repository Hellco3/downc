#include <sys/poll.h>
#include "ProcessBar.h"

ProcessBar::ProcessBar()
    : done_size(0), total_size(0), running(false)
{
}

ProcessBar::~ProcessBar()
{
    if (running)
        pthread_join(pid, nullptr);
}

void ProcessBar::start(uint64_t size, const std::string &file_name)
{
    total_size = size;
    running = true;
    _file_name = file_name;
    pthread_create(&pid, nullptr, thread_func, (void *)this);
    pthread_detach(pid);
}
void ProcessBar::add(uint64_t num)
{
    evtfd.Write(num);
}

void ProcessBar::stop()
{
    sleep(1);
    running = false;
}

void *ProcessBar::thread_func(void *data)
{
    ProcessBar *instance = (ProcessBar *)data;

    struct pollfd fds[1024] = {0};
    int len = sizeof(fds) / sizeof(fds[0]);
    fds[0].fd = instance->evtfd.fd();
    fds[0].events = POLLIN | POLLERR | POLLHUP;
    const int slice = 4096;
    uint32_t buflen = 0;
    while (instance->running)
    {
        int n = poll(fds, len, -1);
        for (int i = 0; i < len && n-- > 0; ++i)
        {
            if (fds[i].revents && POLLIN)
            {
                uint64_t val = instance->evtfd.Read();
                buflen += val;
                if (buflen >= slice || instance->total_size - instance->done_size == buflen)
                {
                    instance->done_size += buflen;
                    if (instance->done_size > instance->total_size)
                    {
                        instance->done_size = instance->total_size;
                    }
                    double percents = 100 * (double)instance->done_size / instance->total_size;
                    printf("\r%s[%.2lf%%]", instance->_file_name.c_str(), percents);
                    // std::cout << "\r"
                    //           << "[" << std::fixed << std::setprecision(2)
                    //           << percents << "\%"
                    //           << "]" << std::flush; // 在这一行的首位置打印出进度条百分比
                    for (int j = 1; j <= percents; j++)
                    {
                        printf(">");
                    }
                    fflush(stdout);
                    if (instance->done_size == instance->total_size)
                    {
                        std::cout << std::endl;
                        instance->running = false;
                        break;
                    }
                    buflen = 0;
                }
            }
        }
    }
    return nullptr;
}