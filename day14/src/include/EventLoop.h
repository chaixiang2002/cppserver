#pragma once
#include "Macros.h"
#include <functional>
class Epoll;
class Channel;
class ThreadPool;
// 会创建一个epoll,然后循环处理事件
// 一个线程池，负责处理任务
class EventLoop
{
private:
    Epoll *ep{nullptr};
    // ThreadPool *threadPool;
    bool quit{false};
public:
    EventLoop();
    ~EventLoop();
    DISALLOW_COPY_AND_MOVE(EventLoop);
    
    void loop();
    void updateChannel(Channel*);

    void addThread(std::function<void()>);
};

