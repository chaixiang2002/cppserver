
#pragma once
#include "Macros.h"
#include <sys/epoll.h>
#include <functional>
class EventLoop;
class Channel
{
private:
    EventLoop *loop;// epoll循环
    int fd;//客户端fd
    uint32_t listen_events_;// 类型（ET或者LT）
    uint32_t ready_events_;// 事件int
    bool inEpoll;
    std::function<void()> read_callback; // 回调函数
    std::function<void()> write_callback; // 回调函数
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();
    DISALLOW_COPY_AND_MOVE(Channel)

    void handleEvent();
    void enableReading();

    int getFd();
    uint32_t getListenEvents();
    uint32_t getReadyEvents();
    bool getInEpoll();
    void setInEpoll(bool in =true);
    void UseET();

    // void setEvents(uint32_t);
    void setReadyEvents(uint32_t);
    void setReadCallback(std::function<void()> const &callback);
};

