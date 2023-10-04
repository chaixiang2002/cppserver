#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "util.h"


int main(){
    int i;
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    errif(sockfd==-1,"socket create error");

    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    client_addr.sin_family=AF_INET;
    client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    client_addr.sin_port=htons(8088);

    i=connect(sockfd,(sockaddr*)&client_addr,sizeof(client_addr));
    errif(i==-1,"socket connect error");

    while (true)
    {
        char buf[1024];
        bzero(&buf,sizeof(buf));
        scanf("%s", buf);
        ssize_t read_bytes=read(sockfd,buf,sizeof(buf));
        if(read_bytes>0){
            printf("message from server: %s\n", buf);          // 打印buf
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes == -1){
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}