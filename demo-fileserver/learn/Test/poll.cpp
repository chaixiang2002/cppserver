#include "mysocket.h"
#include "util.h"
#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

void fun(){
    auto [ip,port] =read_config("server");
    mysocket serv_sock(AF_INET,SOCK_STREAM);
    serv_sock._bind(AF_INET,ip.c_str() ,port );
    serv_sock._listen(SOMAXCONN);

    int listenfd=serv_sock.getsockfd();

    // pollfd init
    pollfd fds[2048];
    for (int i=0; i<2048; i++) {//  
        fds[i].fd=-1;
    }
    fds[listenfd].fd=listenfd;
    fds[listenfd].events=POLLIN;//表示为读事件 POLLOUT为写事件 或者 POLLIN|POLLOUT。
    int maxfd=listenfd;



    while (true) {
        // timeval timeout;
        // timeout.tv_sec=20;
        // timeout.tv_usec=0;

        int infds=poll(fds, maxfd+1, 20000);// 超时20秒
        LOG("poll")
        if (infds<0) {
            perror("poll() failed");
            break;
        }else if (infds==0) {// 太久没事件，关闭服务器！
            print("poll time out");
            break;
        }else {
            LOG("有读事件")

            for (int eventfd=0; eventfd<=maxfd; eventfd++) {
                if (fds[eventfd].fd==-1 || (fds[eventfd].revents&POLLIN)==0) continue; //忽略该fd 或者 没有读事件

                if(eventfd==listenfd){// 服务器的监听sock
                    LOG("服务器有读事件")

                    shared_ptr<mysocket> client_mysock=serv_sock._accpet();///!!!!出作用域会消失
                    int clientfd=client_mysock->getsockfd();
                    client_mysock->move_();//

                    misjudgment(clientfd,"serv_sock._accpet() failed");

                    //
                    fds[clientfd].fd=clientfd;
                    fds[clientfd].events=POLLIN;
                    if (maxfd<clientfd) {
                        maxfd=clientfd;//更新最大fd;
                    }
                    
                }else {// 客户端的处理sock
                    LOG("客户端有读事件")

                    char buf[1024];
                    memset(&buf, 0, sizeof(buf));
                    // if (recv(eventfd, buf, 5, 0)<=0) {
                    if (recv(eventfd, buf, sizeof(buf), 0)<=0) {
                        printf("client[%d]:已断开连接\n",eventfd);
                        
                        close(eventfd);
                        fds[eventfd].fd=-1;

                        if (eventfd==maxfd) {
                            for (int i=eventfd; i>0; i--) {
                                if(fds[i].fd!=-1){
                                    maxfd=i;// 更新最大fd
                                    break;
                                }
                            }
                        }
                    }else {
                        printf("client[%d]:%s\n",eventfd,buf);
                        // send(eventfd, buf, strlen(buf), 0);
                        send(eventfd, buf, sizeof(buf), 0);
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