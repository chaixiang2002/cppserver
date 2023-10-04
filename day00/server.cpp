#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string.h>
#include <stdio.h>


int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(8088);

    bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr));


    listen(sockfd,SOMAXCONN);//SOMAXCONN： listen函数的最大监听队列长度，系统建议的最大值SOMAXCONN被定义为128。

    struct sockaddr_in clnt_addr;
    socklen_t clint_addr_len=sizeof(clnt_addr);
    bzero(&clnt_addr,sizeof(clnt_addr));
    int clnt_sockfd=accept(sockfd,(sockaddr*)&clnt_addr,&clint_addr_len);

    printf("new client fd: %d   IP: %d   Port: %d  \n",clnt_sockfd,inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port));
    return 0;
}