#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


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

    struct sockaddr_in clnt_addr;
    socklen_t clint_addr_len=sizeof(clnt_addr);
    bzero(&clnt_addr,sizeof(clnt_addr));
    int clnt_sockfd=accept(sockfd,(sockaddr*)&clnt_addr,&clint_addr_len);
    errif(clnt_sockfd==-1,"socket accept error");

    printf("new client fd: %d   IP: %d   Port: %d  \n",clnt_sockfd,inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port));
    while (true)
    {
        char buf[1024];
        bzero(&buf,sizeof(buf));
        ssize_t read_bytes=read(clnt_sockfd,buf,sizeof(buf));// <unistd.h>
        if (read_bytes>0)
        {
            printf("message from client fd %d: %s\n",clnt_sockfd,buf);
            write(clnt_sockfd,buf,sizeof(buf));
        } else if(read_bytes == 0){
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        } else if(read_bytes == -1){
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
        
    }
    close(sockfd);
    return 0;
}