#include "mysocket.h"
#include "util.h"
#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

void fun(){
    auto [ip,port] =read_config("server");
    mysocket serv_sock(AF_INET,SOCK_STREAM);
    serv_sock._bind(AF_INET,ip.c_str() ,port );
    serv_sock._listen(SOMAXCONN);

    int listenfd=serv_sock.getsockfd();

    // select
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(listenfd, &readset);

    int maxfd=listenfd;

    while (true) {
        timeval timeout;
        timeout.tv_sec=20;
        timeout.tv_usec=0;

        fd_set tmpfds=readset;
        // select非阻塞，timeout就是不设置超时，一直运行
        int infds=select(maxfd+1, &tmpfds, NULL, NULL, &timeout);
        if (infds<0) {
            perror("select() failed");
            break;
        }else if (infds==0) {// 太久没事件，关闭服务器！
            print("time out");
            break;
        }else {
            for (int eventfd=0; eventfd<=maxfd; eventfd++) {
                if (FD_ISSET(eventfd, &tmpfds)==0) continue; //该fd无事发生

                if(eventfd==listenfd){// 服务器的监听sock
                    shared_ptr<mysocket> client_mysock=serv_sock._accpet();///!!!!出作用域会消失
                    int clientfd=client_mysock->getsockfd();
                    client_mysock->move_();//

                    misjudgment(clientfd,"serv_sock._accpet() failed");
                    FD_SET(clientfd, &readset);// 加入位图
                    if (maxfd<clientfd) {
                        maxfd=clientfd;//更新最大fd;
                    }
                    
                }else {// 客户端的处理sock
                    char buf[1024];
                    memset(&buf, 0, sizeof(buf));
                    // if (recv(eventfd, buf, 5, 0)<=0) {
                    if (recv(eventfd, buf, sizeof(buf), 0)<=0) {
                        printf("client[%d]:已断开连接\n",eventfd);
                        
                        close(eventfd);
                        FD_CLR(eventfd, &readset);// 移除位图

                        if (eventfd==maxfd) {
                            for (int i=eventfd; i>0; i--) {
                                if(FD_ISSET(i, &readset)){
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