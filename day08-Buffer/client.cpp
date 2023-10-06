#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "include/util.h"

#include "include/Socket.h"
#include "include/InetAddress.h"

#define BUFFER_SIZE 1024
int main(){
    // int i;
    // int sockfd=socket(AF_INET,SOCK_STREAM,0);
    // errif(sockfd==-1,"socket create error");
    Socket *clnt_sock=new Socket(); 

    // struct sockaddr_in client_addr;
    // bzero(&client_addr,sizeof(client_addr));
    // client_addr.sin_family=AF_INET;
    // client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    // client_addr.sin_port=htons(8088);
    InetAddress *clnt_addr=new InetAddress("127.0.0.1",1234);

    // i=connect(sockfd,(sockaddr*)&client_addr,sizeof(client_addr));
    // errif(i==-1,"socket connect error");
    clnt_sock->connect(clnt_addr);

    int sockfd=clnt_sock->getFd();
    while (true)
    {
        char buf[BUFFER_SIZE];  //在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server: %s\n", buf);
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