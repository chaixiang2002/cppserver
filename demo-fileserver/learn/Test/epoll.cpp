#include "mysocket.h"
#include "util.h"
#include <algorithm>
#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

void fun(){
    auto [ip,port] =read_config("server");
    mysocket serv_sock(AF_INET,SOCK_STREAM);
    serv_sock._bind(AF_INET,ip.c_str() ,port );
    serv_sock._listen(SOMAXCONN);
    serv_sock.setnonblocking();

    int listenfd=serv_sock.getsockfd();

    // epoll对象
    int epollfd=epoll_create(1);

    epoll_event ev;// epoll事件对象
    ev.data.fd=listenfd;
    ev.events=EPOLLIN;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
    const int MAXNUM=1024;
    epoll_event events[MAXNUM];// epoll事件数组对象,存放epoll返回的事件

    while (true) {
        // timeval timeout;
        // timeout.tv_sec=20;
        // timeout.tv_usec=0;

        int infds=epoll_wait(epollfd, events, MAXNUM, 20000);// 超时20秒
        LOG("epoll_wait")
        if (infds<0) {
            perror("epoll_wait() failed");
            break;
        }else if (infds==0) {// 太久没事件，关闭服务器！
            print("epoll_wait time out");
            break;
        }else {
            LOG("有读事件")

            for (int i=0; i<infds; i++) {
                if(events[i].data.fd==listenfd){// 服务器的监听sock
                    LOG("服务器有读事件")

                    shared_ptr<mysocket> client_mysock=serv_sock._accpet();///!!!!出作用域会消失
                    int clientfd=client_mysock->getsockfd();
                    client_mysock->move_();//

                    misjudgment(clientfd,"serv_sock._accpet() failed");

                    //
                    ev.data.fd=clientfd;
                    ev.events=EPOLLIN;
                    epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
                }else {// 客户端的处理sock
                    LOG("客户端有读事件")

                    char buf[1024];
                    memset(&buf, 0, sizeof(buf));
                    // if (recv(i, buf, 5, 0)<=0) {
                    if (recv(events[i].data.fd, buf, sizeof(buf), 0)<=0) {
                        printf("client[%d]:已断开连接\n",events[i].data.fd);
                        
                        close(events[i].data.fd);
                        // 关闭sockfd后，epoll会自动剔除改sockfd，不用手动添加
                    }else {
                        printf("client[%d]:%s\n",events[i].data.fd,buf);
                        // send(i, buf, strlen(buf), 0);
                        send(events[i].data.fd, buf, sizeof(buf), 0);
                    }
                }
            } 
        }
    }
}

int main(int argc,char *argv[]){
    fun();
    return 0;
}