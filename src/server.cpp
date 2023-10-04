#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "util.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd){// ??????? 不熟？
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK);
}
int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    errif(sockfd==-1,"socket create error");
    
    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(8088);

    errif(bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr))==-1,"socket bind error");
    
    int i=listen(sockfd,SOMAXCONN);//SOMAXCONN： listen函数的最大监听队列长度，系统建议的最大值SOMAXCONN被定义为128。
    errif(i==-1,"socket listen error");

    /**
     * 
    */
    int epfd=epoll_create1(0);
    errif(epfd==-1,"epoll create error");

    struct epoll_event events[MAX_EVENTS],ev;
    bzero(&events,sizeof(events));
    bzero(&ev,sizeof(ev));
    ev.data.fd=sockfd;
    ev.events=EPOLLIN | EPOLLET;
    setnonblocking(sockfd);
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);

    while (true)
    {
        int nfds=epoll_wait(epfd,events,MAX_EVENTS,-1);
        errif(nfds==-1,"epoll wait error");
        for (int i = 0; i < nfds; i++ )
        {                   
            if(events[i].data.fd==sockfd){
                struct sockaddr_in clnt_addr;
                // bzero(&clnt_addr,sizeof(clnt_addr));
                socklen_t clint_addr_len=sizeof(clnt_addr);
                bzero(&clnt_addr,sizeof(clnt_addr));


                int clnt_sockfd=accept(sockfd,(sockaddr*)&clnt_addr,&clint_addr_len);
                errif(clnt_sockfd==-1,"socket accept error");
                printf("new client fd: %d   IP: %d   Port: %d  \n",clnt_sockfd,inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port));

                bzero(&ev,sizeof(ev));
                ev.data.fd=clnt_sockfd;
                ev.events=EPOLLIN | EPOLLET;
                setnonblocking(clnt_sockfd);
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sockfd,&ev);
            }else if (events[i].events & EPOLLIN)   
            {
                char buf[1024];
                while (true){
                    bzero(&buf,sizeof(buf));
                    ssize_t read_bytes=read(events[i].data.fd,buf,sizeof(buf));// <unistd.h>
                    if (read_bytes>0)
                    {
                        printf("message from client fd %d: %s\n",events[i].data.fd,buf);
                        // ...
                        write(events[i].data.fd,buf,sizeof(buf));
                    }else if (read_bytes==-1 && ((errno==EAGAIN)||(errno==EWOULDBLOCK)))
                    {
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    }else if(read_bytes == 0){
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    } 
                }
            }else{//其他事件，之后的版本实现
                printf("something else happened\n");
            }
        }
        
    }

    close(sockfd);
    return 0;
}