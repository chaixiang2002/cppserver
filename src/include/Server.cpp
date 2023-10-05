#include "Server.h"
#include "Socket.h" 
#include "InetAddress.h"
#include "Channel.h"
#include <string.h>
#include <unistd.h>
#include "Acceptor.h"

#define READ_BUFFER 1024
using std::function;

/**
 * 1. new Acceptor
 * 2. 把新连接处理函数 传给 acceptor
*/
Server::Server(EventLoop* _loop):loop(_loop)
{
    // Socket *serv_sock=new Socket();
    // InetAddress *serv_addr=new InetAddress("127.0.0.1",8088);
    // serv_sock->bind(serv_addr);
    // serv_sock->listen();
    // serv_sock->setnonblocking();

    // Channel *serv_channel=new Channel(loop,serv_sock->getFd());
    // function<void()> fun=std::bind(&Server::newConnection,this,serv_sock);// 绑定函数
    // serv_channel->setCallBack(fun);// 给serv_channel绑定函数
    // serv_channel->enableReading();// 让serv_channel可读：serv_channel.loop.epoll.add(serv_channel)
    acceptor=new Acceptor(loop);
    function<void(Socket*)> fun=std::bind(&Server::newConnection,this,std::placeholders::_1);//??????????????????????????????
    acceptor->setNewConnectionCallback(fun);

}

Server::~Server()
{
    delete acceptor;
}
void Server::handleReadEvent(int sockfd){
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}
void Server::newConnection(Socket *serv_sock){
    InetAddress *clnt_addr=new InetAddress();
    Socket *clnt_sock=new Socket(serv_sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();

    Channel *clnt_channel=new Channel(loop,clnt_sock->getFd());
    function<void()> fun=std::bind(&Server::handleReadEvent,this,clnt_sock->getFd());
    clnt_channel->setCallBack(fun);
    clnt_channel->enableReading();
}