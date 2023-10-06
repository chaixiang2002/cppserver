#pragma once
class Epoll;
class Channel;

// 会创建一个epoll,然后循环处理事件
class EventLoop
{
private:
    Epoll *ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);
};

