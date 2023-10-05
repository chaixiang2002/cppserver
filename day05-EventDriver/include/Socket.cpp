#include "Socket.h"
#include "sys/socket.h"
#include "util.h"
#include <unistd.h>
#include "InetAddress.h"
#include <fcntl.h>

Socket::Socket()
{   
    fd=socket(AF_INET,SOCK_STREAM,0);
    errif(fd==-1,"socket create error");
}

Socket::Socket(int _fd):fd(_fd)
{   
    errif(_fd==-1,"socket create error");
}

Socket::~Socket()
{
    if(fd!=-1){
        close(fd);
        fd=-1;
    }
}

void Socket::bind(InetAddress *addr){
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind error");
}
void Socket::connect(InetAddress* addr){
    errif(::connect(fd,(sockaddr*)&addr->addr, addr->addr_len)==-1,"socket connect error");
}

void Socket::listen(){
    errif(::listen(fd,SOMAXCONN)==-1,"socket listen error");
}

void Socket::setnonblocking(){
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr){
    int clnt_socketfd=::accept(fd,(sockaddr*)&addr->addr,&addr->addr_len);
    errif(clnt_socketfd==-1,"socket error accept");
    return clnt_socketfd;
}

int Socket::getFd(){
    return fd;
}
