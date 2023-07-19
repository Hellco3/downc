#include "Eventfd.h"


Eventfd::Eventfd()
{
    evtfd = eventfd(0, 0);
}

void Eventfd::Write(uint64_t value)
{
    eventfd_write(evtfd, value);
}

uint64_t Eventfd::Read()
{
    eventfd_t value;
    eventfd_read(evtfd, &value);
    return value;
}

Eventfd::~Eventfd()
{
    close(evtfd);
}