// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <string.h>
// #include <unistd.h>
// #include <stdio.h>
// #include "include/util.h"

// #include "include/Socket.h"
// #include "include/InetAddress.h"
// #include "include/Buffer.h"

// // #define BUFFER_SIZE 1024
// int main(){
//     // int i;
//     // int sockfd=socket(AF_INET,SOCK_STREAM,0);
//     // errif(sockfd==-1,"socket create error");
//     Socket *clnt_sock=new Socket(); 

//     // struct sockaddr_in client_addr;
//     // bzero(&client_addr,sizeof(client_addr));
//     // client_addr.sin_family=AF_INET;
//     // client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
//     // client_addr.sin_port=htons(8088);
//     InetAddress *clnt_addr=new InetAddress("127.0.0.1",1234);

//     // i=connect(sockfd,(sockaddr*)&client_addr,sizeof(client_addr));
//     // errif(i==-1,"socket connect error");
//     clnt_sock->connect(clnt_addr);

//     Buffer *acppetBuffer =new Buffer();
//     Buffer *sendBuffer =new Buffer();

//     int sockfd=clnt_sock->getFd();
//     while (true)
//     {
//         sendBuffer->getline();
//         ssize_t write_bytes=write(sockfd,sendBuffer->c_str(),sendBuffer->size());
//         if(write_bytes==-1){
//                 printf("socket already disconnected, can't write any more!\n");
//                 break;
//         }
//         char buf[1024];
//         int already_read = 0;
//         while (true)
//         {
//             ssize_t read_bytes=read(sockfd,buf,sizeof(buf));
//             if (read_bytes>0)
//             {
//                 acppetBuffer->append(buf,sizeof(buf));
//                 already_read+=read_bytes;
//             }else if (read_bytes==0)
//             {
//                 printf("server disconnected!\n");
//                 exit(EXIT_SUCCESS);
//             }
//             if(already_read>=sendBuffer->size()){
//                 printf("message from server: %s\n", acppetBuffer->c_str());
//                 break;
//             }
//         }
//         acppetBuffer->clear();
//     }
//     close(sockfd);

//     delete clnt_addr;
//     delete clnt_sock;
//     delete acppetBuffer;
//     delete sendBuffer;

//     return 0;
// }

#include "include/Client.h"

int main(){

    Client * client=new Client(10000,2);
    return 0;
}