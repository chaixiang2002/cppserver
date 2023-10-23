#pragma once
#include "Macros.h"
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
private:
    int epfd{1};
    struct epoll_event *events{nullptr};
public:
    Epoll();
    ~Epoll();
    DISALLOW_COPY_AND_MOVE(Epoll);

    // void addFd(int fd,uint32_t op);
    void updateChannel(Channel*);
    void deleteChannel(Channel*);

    std::vector<Channel*> poll(int timeout=-1);
};

