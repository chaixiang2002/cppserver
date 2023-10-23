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
    // InetAddress *addr;
    Channel *acceptChannel;
    std::function<void(Socket*)> newConnectCallback;

public:
    explicit Acceptor(EventLoop *_loop);//不让C++的隐式类型转换
    ~Acceptor();
    DISALLOW_COPY_AND_MOVE(Acceptor);
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)> const &callback);
};

