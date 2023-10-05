#pragma once
#include <functional>
class EventLoop;
class Socket;
class InetAddress;
class Channel;

/**
 * new sockert
 * new addr
 * new channel
 * 绑定现有的loop
 * 
 * std::function<void(Socket*)> newConnectionCallback；
 * setNewConnectionCallback()设置setNewConnectionCallback
*/
class Acceptor
{
private:
    EventLoop *loop;
    Socket *sock;
    InetAddress *addr;
    Channel *acceptChannel;
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket*)> newConnectCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};

