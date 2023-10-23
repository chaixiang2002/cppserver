#include "Client.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Buffer.h"
#include <unistd.h>
#include <string.h>
Client::Client(int msgs, int wait)
{
    sock=new Socket();
    addr=new InetAddress("127.0.0.1",1234);
    sock->connect(addr);
    acceptBuffer =new Buffer();
    sendBuffer =new Buffer();
    int sockfd=sock->getFd();
    sleep(wait);
    int count = 0;

    while (count<msgs)
    {
        sendBuffer->setBuf("I'm client!");
        ssize_t write_bytes=write(sockfd,sendBuffer->ToStr(),sendBuffer->size());
        if(write_bytes==-1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[1024];    //这个buf大小无所谓
        while(true){
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if(read_bytes > 0){
                acceptBuffer->append(buf, read_bytes);
                already_read += read_bytes;
            } else if(read_bytes == 0){         //EOF
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if(already_read >= sendBuffer->size()){
                printf("count: %d, message from server%d: %s\n", count++,sock->getFd() , acceptBuffer->ToStr());
                break;
            } 
        }
        acceptBuffer->clear();
    }
    close(sockfd);
}

void Client::oneClient(int msgs, int wait){
    Client * a=new Client(msgs,wait);
    delete a;
}

Client::~Client()
{
    delete sock;
    delete addr;
    delete sendBuffer;
    delete acceptBuffer;
}
