// #pragma once
// #include <sys/epoll.h>
// #include <functional>
// // class Epoll;
// class EventLoop;
// class Channel
// {
// private:
//     // Epoll *ep;
//     EventLoop *loop;// epoll循环
//     int fd;//客户端fd
//     uint32_t events;// 类型（ET或者LT）
//     uint32_t revents;// 事件int
//     bool inEpoll;
//     std::function<void()> callback;// 回调函数

// public:
//     Channel(EventLoop *_loop,int _fd);
//     ~Channel();

//     void handleEvent();
//     void enableReading();

//     int getFd();
//     uint32_t getEvents();
//     uint32_t getRevents();
//     bool getInEpoll();
//     void setInEpoll();

//     void setRevents(uint32_t);
//     void setCallBack(std::function<void()>);
// };

#pragma once
#include <sys/epoll.h>
#include <functional>
class EventLoop;
class Channel
{
private:
    EventLoop *loop;// epoll循环
    int fd;//客户端fd
    uint32_t events;// 类型（ET或者LT）
    uint32_t revents;// 事件int
    bool inEpoll;
    std::function<void()> callback; // 回调函数
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    // void setEvents(uint32_t);
    void setRevents(uint32_t);
    void setCallBack(std::function<void()>);
};

