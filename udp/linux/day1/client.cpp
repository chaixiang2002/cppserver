#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "util.h"

#define BUFFER_SIZE 1024
 
int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    errif(sockfd == -1, "socket创建错误");

    struct sockaddr_in server_addr=get_addr("/root/code/cppserver/udp/linux/day1/client_config.txt");
    // bzero(&server_addr, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // // server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    // // server_addr.sin_addr.s_addr = inet_addr("192.168.0.14");
    // server_addr.sin_addr.s_addr = inet_addr("124.71.45.252");
    // server_addr.sin_port = htons(8088);
    
    while (true) {
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);

        ssize_t send_bytes = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        errif(send_bytes == -1, "socket发送错误");

        struct sockaddr_in server_response_addr;
        socklen_t server_response_addr_len = sizeof(server_response_addr);
        bzero(&server_response_addr, sizeof(server_response_addr));
        bzero(&buf, sizeof(buf));

        ssize_t recv_bytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&server_response_addr, &server_response_addr_len);
        errif(recv_bytes == -1, "socket接收错误");
        
        printf("来自服务器的消息：%s\n", buf);
    }

    close(sockfd);
    return 0;
}