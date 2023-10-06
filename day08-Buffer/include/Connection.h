#pragma once
#include <functional>
#include "Buffer.h"
class EventLoop;
class Socket;
class Channel;


/**
 * 1. 绑定EventLoop
 * 2. 绑定Socket
 * 3. 创建Channel
 * 4. 一个删除连接的函数
*/
class Connection
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(Socket*)> deleteConnectionCallback;
    std::string *inBuffer;
    Buffer *readBuffer;
public:
    Connection(EventLoop *_loop,Socket *_sock);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);

};

