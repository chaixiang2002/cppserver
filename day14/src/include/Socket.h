#pragma once //???????
#include "Macros.h"
#include <sys/types.h>
class InetAddress;
class Socket
{
private:
    int fd{-1};
public:
    Socket();
    explicit Socket(int);
    ~Socket();
    DISALLOW_COPY_AND_MOVE(Socket);

    void bind(InetAddress*);
    void listen();
    
    void setnonblocking();
    bool IsNonBlocking();

    void connect(InetAddress*);
    void connect(const char *ip,uint16_t port);

    int accept(InetAddress*);
    int getFd();
};


