#pragma once
#include <map>
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
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int,Connection*> connections;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);//????实现呢？？
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
};


