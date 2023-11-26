#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define BUFFER_SIZE 1024

void errif(int condition, const char* message) {
    if (condition) {
        fprintf(stderr, "%s\n", message);
        exit(1);
    }
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    errif(result != 0, "WSAStartup 失败");

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    errif(sockfd == INVALID_SOCKET, "socket创建错误");

    struct sockaddr_in server_addrstruct sockaddr_in server_addr=get_addr("client_config.txt");
    // memset(&server_addr, 0, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // server_addr.sin_addr.s_addr = inet_addr("124.71.45.252");
    // server_addr.sin_port = htons(8088);

    while (1) {
        char buf[BUFFER_SIZE];
        memset(buf, 0, sizeof(buf));
        scanf("%s", buf);

        int send_bytes = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        errif(send_bytes == SOCKET_ERROR, "socket发送错误");

        struct sockaddr_in server_response_addr;
        int server_response_addr_len = sizeof(server_response_addr);
        memset(&server_response_addr, 0, sizeof(server_response_addr));

        memset(buf, 0, sizeof(buf));
        int recv_bytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&server_response_addr, &server_response_addr_len);
        errif(recv_bytes == SOCKET_ERROR, "socket接收错误");

        printf("来自服务器的消息：%s\n", buf);
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}