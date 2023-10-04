#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string.h>
#include <stdio.h>


int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    client_addr.sin_family=AF_INET;
    client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    client_addr.sin_port=htons(8088);

    connect(sockfd,(sockaddr*)&client_addr,sizeof(client_addr));

    return 0;
}