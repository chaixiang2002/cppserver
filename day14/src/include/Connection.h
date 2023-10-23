#pragma once
#include <functional>
#include "Buffer.h"
#include "Macros.h"
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
public:
    enum State{
        Invalid=1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
    Connection(EventLoop *_loop,Socket *_sock);
    ~Connection();

    DISALLOW_COPY_AND_MOVE(Connection);
    void Read();
    void Write();

    // void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)> const &callback);
    void setOnConnectCallback(std::function<void(Connection*)> const &callback);
    State GetState();
    void close();
    void SetSendBuffer(const char *str);

    Buffer *GetReadBuffer();
    const char *ReadBuffer();
    Buffer *GetSendBuffer();
    const char *SendBuffer();
    
    void GetlineSendBuffer();
    Socket *GetSocket();
    void OnConnect(std::function<void()> fn);

private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel{nullptr};
    State state_{State::Invalid};
    Buffer *readBuffer{nullptr};
    Buffer *sendBuffer{nullptr};
    std::function<void(Socket*)> deleteConnectionCallback;
    std::function<void(Connection*)> on_connect_callback_;

    void ReadNonBlocking();
    void WriteNonBlocking();
    void ReadBlocking();
    void WriteBlocking();


};

