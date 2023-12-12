#include "toml.hpp"
#include "util.h"
#include <asm-generic/socket.h>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <strings.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include <string.h>
#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <functional>
#include <unistd.h>
#include <memory>
using namespace std;
//不需要了
// void setnonblocking(int fd){
//     int stats=fcntl(fd, F_GETFL);// 获得FL
//     fcntl(fd,F_SETFL,stats|O_NONBLOCK);//设置FL
// }

int main(int argc,char *argv[]){
    auto ip=read_config<string>("server", "ip");
    auto port=read_config<int>("server", "port");

    int listenfd=socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);// 可以直接创建非阻塞socck
    sock_judge(listenfd,listenfd,"socket() failed ");
    LOG("socket")
    LOG(ip)
    LOG(port)

    int opt;
    /*
    - 允许重用本地地址
    - 禁用 Nagle’s 算法
    - 允许多个套接字绑定到同一个端口
    - 系统会定期发送保活报文
    */
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt)));
    setsockopt(listenfd, SOL_SOCKET, TCP_NODELAY, &opt, static_cast<socklen_t>(sizeof(opt)));
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof(opt)));
    setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof(opt)));
    
    struct sockaddr_in servaddr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(ip.c_str());
    servaddr.sin_port=htons(port);

    sock_judge(listenfd, ::bind(listenfd, (sockaddr *)&servaddr, sizeof(servaddr)), "bind() failed");
    LOG("bind")
    sock_judge(listenfd, ::listen(listenfd, 128), "listen() failded");
    LOG("listen")

    int epollfd=epoll_create(1);

    epoll_event ev;
    ev.data.fd=listenfd;
    ev.events=EPOLLIN;// 水平触发
    // ev.events=EPOLLIN | EPOLLET;// 边沿触发

    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
    
    epoll_event evs[10];

    while (true) {
        // LOG("epoll_wait")
        int infds=epoll_wait(epollfd, evs, 10, -1);
        if (infds<0) {
            perror("epoll_wait() failed");
            break;
        }else if (infds==0) {
            println("epoll_wait() time_out");
            continue;
        }else {
            for (int i=0; i<infds; i++) {
                if (evs[i].events & EPOLLRDHUP) {// 关闭的事件
                        printf("client[%d]: disconnected\n",evs[i].data.fd);
                        close(evs[i].data.fd);
                }else if (evs[i].events & (EPOLLIN | EPOLLPRI) ) {// 可读事件  （普通数据|带外数据）
                    if (evs[i].data.fd==listenfd) {// 服务器的事件
                        LOG("有服务器事件")
                        struct sockaddr_in clnt_addr;
                        socklen_t clint_addr_len=sizeof(clnt_addr);
                        int clientfd=accept4(listenfd, (sockaddr*)&clnt_addr, &clint_addr_len,O_NONBLOCK);//也是非阻塞了
                        // setnonblocking(clientfd);//
                        printf("new client fd: %d   IP: %s   Port: %d  \n",clientfd,inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port));
                        
                        ev.data.fd=clientfd;
                        // ev.events=EPOLLIN | EPOLLOUT | EPOLLET;
                        ev.events=EPOLLIN | EPOLLOUT ;
                        epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
                    }else {// 客户端的事件
                        // LOG("客户端的事件")
                        char buf[1024];
                        while (true) {// 边沿触发 且 非阻塞Sock
                            bzero(&buf,sizeof(buf));
                            ssize_t nread=read(evs[i].data.fd,buf,sizeof(buf));
                            if (nread>0) { //
                                printf("client[%d]:%s\n",evs[i].data.fd,buf);
                                send(evs[i].data.fd, buf, strlen(buf), 0);
                            }else if (nread==-1 && errno==EINTR) {// 读取数据时被 信号截断，继续读取
                                continue;
                            }else if (nread==-1 && (errno==EAGAIN || errno==EWOULDBLOCK)) {//全部读取完毕
                                break;
                            }else if(nread==0){// 客户端已断开
                            // }else if(errno==0){// 客户端已断开
                                printf("client[%d]:已断开连接\n",evs[i].data.fd);
                                close(evs[i].data.fd);
                                break;
                            }
                        }
                    }
                }else if (evs[i].events & EPOLLOUT) {// 写事件
                    // todo
                    // LOG("to do 。。。")    
                }else { // 其他事件，均视为错误
                    printf("client[%d]:其他，均视为错误\n",evs[i].data.fd);
                    close(evs[i].data.fd);
                    break;
                }
            }
        }
    }
    
    
    
    
    
    
    
    
    return 0;
}