#pragma once
#include <arpa/inet.h>

class InetAddress
{
private:
    
public:
    InetAddress();
    InetAddress(const char* ip,uint16_t port);
    ~InetAddress();
    struct sockaddr_in addr;
    socklen_t addr_len;

};

