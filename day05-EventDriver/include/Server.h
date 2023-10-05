#pragma once
class EventLoop;
class Socket;

// 一个loop循环即可，
// 有处理函数
// 有新连接的函数
class Server
{
private:
    EventLoop *loop;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};


