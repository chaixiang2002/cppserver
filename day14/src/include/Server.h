#pragma once
#include "Macros.h"
#include "ThreadPool.h"
#include <functional>
#include <map>
#include <vector>
class EventLoop;
class Socket;
class Acceptor;
class Connection;

/**
 * 1. 需绑定现有的loop
 * 2. new Acceptor
 * 3. 2种事件处理函数，一个处理新连接事件，一个处理可读事件
*/
class Server
{
private:
    EventLoop *main_reactor;
    Acceptor *acceptor;
    std::map<int,Connection*> connections;
    std::vector<EventLoop*>sub_reactors;
    ThreadPool *threadPool;
    std::function<void(Connection *)> on_connect_callback_;
public:
    explicit Server(EventLoop*);
    ~Server();
    DISALLOW_COPY_AND_MOVE(Server);

    // void handleReadEvent(int);//????实现呢？？
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
    void OnConnect(std::function<void(Connection *)> fn);
};


