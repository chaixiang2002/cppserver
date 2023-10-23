#include "Socket.h"
#include "sys/socket.h"
#include "util.h"
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
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
    struct sockaddr_in tmp_addr=addr->GetAddr();
    errif(::bind(fd, (sockaddr*)&tmp_addr, sizeof(tmp_addr)) == -1, "socket bind error");
}
/*
总体来说，这段代码在非阻塞模式下，通过不断尝试连接服务器直到连接成功，
或在阻塞模式下直接连接服务器。如果连接失败，则抛出一个错误。
*/
void Socket::connect(InetAddress* addr){
    struct sockaddr_in tmp_addr=addr->GetAddr();
    if(fcntl(fd, F_GETFL)& O_NONBLOCK){//在非阻塞模式下
        while (true) {
            int ret=::connect(fd, (sockaddr*)&tmp_addr, sizeof(tmp_addr));
            if (ret==0) {
                break;  
            }
            if (ret==-1 && (errno==EINPROGRESS)) {
                continue;
            }
            if (ret == -1) {
                errif(true, "socket connect error");
            }
        }

    }else {//在阻塞模式下
        errif(::connect(fd,(sockaddr*)&tmp_addr, sizeof(tmp_addr))==-1,"socket connect error");   
    }
}

void Socket::listen(){
    errif(::listen(fd,SOMAXCONN)==-1,"socket listen error");
}

void Socket::setnonblocking(){
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK);
}

bool Socket::IsNonBlocking(){
    return (fcntl(fd, F_GETFL)& O_NONBLOCK)!=0; 
}

int Socket::accept(InetAddress *addr){
    // int clnt_socketfd=::accept(fd,(sockaddr*)&addr->addr,&addr->addr_len);
    // errif(clnt_socketfd==-1,"socket error accept");
    // return clnt_socketfd;
    int clnt_socketfd=-1;
    struct sockaddr_in tmp_addr {};
    socklen_t addr_len=sizeof(tmp_addr);
    if (IsNonBlocking()) {
        while (true) {
            clnt_socketfd=::accept(fd, (sockaddr*)&tmp_addr, &addr_len);
            if (clnt_socketfd==-1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
                continue;
            }
            if (clnt_socketfd==-1) {
                errif(true,"socket accept error");
            }else {
                break;
            }
        }
    }else {
        clnt_socketfd=::accept(fd, (sockaddr*)&tmp_addr, &addr_len);
        errif(clnt_socketfd==-1, "socket accept error");
    }
    addr->SetAddr(tmp_addr);
    return clnt_socketfd;
}

void Socket::connect(const char*ip,uint16_t port){
    InetAddress *addr=new InetAddress(ip,port);
    connect(addr);
    delete addr;
}

int Socket::getFd(){
    return fd;
}
