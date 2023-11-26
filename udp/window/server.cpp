#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define READ_BUFFER 1024

int main() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("socket创建错误\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serv_addrget_addr("server_config.txt");;
    // memset(&serv_addr, 0, sizeof(serv_addr));
    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // // serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // serv_addr.sin_port = htons(8088);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("socket绑定错误\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    while (1) {
        struct sockaddr_in clnt_addr;
        int clnt_addr_len = sizeof(clnt_addr);
        memset(&clnt_addr, 0, sizeof(clnt_addr));

        char buf[READ_BUFFER];
        memset(&buf, 0, sizeof(buf));
        int recv_bytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
        if (recv_bytes == SOCKET_ERROR) {
            printf("socket接收错误\n");
            break;
        }

        char clnt_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clnt_addr.sin_addr), clnt_ip, INET_ADDRSTRLEN);
        printf("来自客户端 IP: %s   Port: %d 的消息\n", clnt_ip, ntohs(clnt_addr.sin_port));

        // 处理接收到的消息
        printf("接收到的消息：%s\n", buf);

        // 将消息回传给客户端
        int send_bytes = sendto(sockfd, buf, recv_bytes, 0, (struct sockaddr*)&clnt_addr, clnt_addr_len);
        if (send_bytes == SOCKET_ERROR) {
            printf("socket发送错误\n");
            break;
        }
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
