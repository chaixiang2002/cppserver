#include "InetAddress.h"
#include <string.h>

InetAddress::InetAddress()=default;
InetAddress::InetAddress(const char*ip ,uint16_t port){
    // bzero(&addr,sizeof(addr));
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family=AF_INET;
    addr_.sin_addr.s_addr=inet_addr(ip);
    addr_.sin_port=htons(port);
    // ???
}

// InetAddress::~InetAddress(){}
void InetAddress::SetAddr(sockaddr_in addr){addr_=addr;}

sockaddr_in InetAddress::GetAddr() { return addr_; }

const char *InetAddress::GetIp() { return inet_ntoa(addr_.sin_addr); }

uint16_t InetAddress::GetPort() { return ntohs(addr_.sin_port); }
