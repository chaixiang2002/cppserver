#pragma once
#include <arpa/inet.h>

class InetAddress
{
private:
    struct sockaddr_in addr_ {};
    
public:
    InetAddress();
    InetAddress(const char* ip,uint16_t port);
    ~InetAddress()=default;
    DISALLOW_COPY_AND_MOVE(InetAddress);
    // struct sockaddr_in addr;
    // socklen_t addr_len;
    sockaddr_in GetAddr();
    const char *GetIp();
    uint16_t GetPort();
};

