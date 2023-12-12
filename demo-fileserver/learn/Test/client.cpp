#include "util.h"
#include <cstdlib>
#include <iostream>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <utility>
using namespace std;

void fun(int argc,char *argv[]){
    // create
    int sockfd=socket(AF_INET, SOCK_STREAM, 0);
    misjudgment(sockfd,"create socket fild");
    auto closefd=[=](){close(sockfd);};

    // addr
    sockaddr_in serv_addr;
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);//本机的全部ip

    // bind
    misjudgment (bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)),"bind()",closefd);

    // misjudgment(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)),"connect(addr)");
    misjudgment(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)),"listen()",closefd);  

}

int main(int argc,char *argv[]){
    if (argc!=2) {
        print("参数不合法");
    }
    
    fun(argc,argv);
    return 0;
}