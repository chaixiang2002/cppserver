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

const string req="GET / HTTP/1.1\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\n"
                "Accept-Encoding: gzip, deflate\n"
                "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\n"
                "Cache-Control: max-age=0\n"
                "Connection: keep-alive\n"
                "Host: 192.168.127.130:9007\n"
                "Upgrade-Insecure-Requests: 1\n"
                "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 Edg/119.0.0.0\n";

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

    for(int i=0; i<3;i++){
        bzero(buf, sizeof(buf));
        // printf("client: ");scanf("%s",buf);

        if (send(client_fd, req.c_str(), req.size(), 0)<=0) {
        // if (send(client_fd, buf, strlen(buf), 0)<=0) {
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