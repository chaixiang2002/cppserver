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

void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    errif(sockfd == -1, "socket创建错误");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8088);

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket绑定错误");

    int epfd = epoll_create1(0);
    errif(epfd == -1, "epoll创建错误");

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    setnonblocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll等待错误");
        
        for (int i = 0; i < nfds; i++) {
            printf("%d, %d----------\n", i, events[i].data.fd);
            
            if (events[i].data.fd == sockfd) {
                struct sockaddr_in clnt_addr;
                socklen_t clint_addr_len = sizeof(clnt_addr);
                bzero(&clnt_addr, sizeof(clnt_addr));

                char buf[READ_BUFFER];
                bzero(&buf, sizeof(buf));
                ssize_t recv_bytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&clnt_addr, &clint_addr_len);
                errif(recv_bytes == -1, "socket接收错误");
                printf("来自客户端 IP: %s   Port: %d 的消息\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

                // 处理接收到的消息
                printf("接收到的消息：%s\n", buf);
                
                // 将消息回传给客户端
                ssize_t send_bytes = sendto(sockfd, buf, recv_bytes, 0, (struct sockaddr*)&clnt_addr, clint_addr_len);
                errif(send_bytes == -1, "socket发送错误");
            }
        }
    }

    close(sockfd);
    return 0;
}