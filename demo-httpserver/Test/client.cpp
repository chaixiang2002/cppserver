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

void setnonblocking(int fd){
    int stats=fcntl(fd, F_GETFL);// 获得FL
    fcntl(fd,F_SETFL,stats|O_NONBLOCK);//设置FL
}

int main(int argc,char *argv[]){
    auto ip=read_config<string>("client", "ip");
    auto port=read_config<int>("client", "port");

    struct sockaddr_in clientaddr;
    clientaddr.sin_family=AF_INET;
    clientaddr.sin_addr.s_addr=inet_addr(ip.c_str());
    clientaddr.sin_port=htons(port);

    char buf[1024];
    int client_fd=socket(AF_INET, SOCK_STREAM, 0);


    sock_judge(client_fd, ::connect(client_fd, (sockaddr*)&clientaddr,sizeof(clientaddr)), "connect() failded");//!!!
    LOG("connect ok!")

    for(int i=0; i<200000;i++){
        bzero(buf, sizeof(buf));
        printf("client: ");scanf("%s",buf);

        if (send(client_fd, buf, strlen(buf), 0)<=0) {
            println("send failed");
            close(client_fd);
            return -1;
        }
        bzero(buf, sizeof(buf));
        if (recv(client_fd, buf, sizeof(buf), 0)<=0) {
            println("recv failed");
            close(client_fd);
            return -1;
        }

        printf("msg from server: %s\n",buf);
    }    
    
    return 0;
}