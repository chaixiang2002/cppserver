#include "util.h"
#include "stdlib.h"
#include "stdio.h"
#include <arpa/inet.h>
#include <cstddef>
#include <string.h>
void errif(bool condition,const char *errmsg){
    if(condition){
        perror("errmsg");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in get_addr(const char *filename){
    struct sockaddr_in serv_addr;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开配置文件\n");
        printf("%s\n",filename);
        printf("无法打开配置文件\n");

        return serv_addr;
    }

    char ip[16];
    int port;
    fscanf(file, "IP=%s\nPORT=%d", ip, &port);
    fclose(file);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    // serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    // serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    serv_addr.sin_port = htons(port);
    return serv_addr;
}