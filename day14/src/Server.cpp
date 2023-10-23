#include "Server.h"
#include "EventLoop.h"
#include "Socket.h" 
#include "InetAddress.h"
#include "Channel.h"
#include <cstdint>
#include <functional>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"
#include "util.h"

#define READ_BUFFER 1024
using std::function;

/**
 * 1. new Acceptor
 * 2. 把新连接处理函数 传给 acceptor
*/
Server::Server(EventLoop* _loop):main_reactor(_loop)
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
    acceptor=new Acceptor(main_reactor);
    function<void(Socket*)> fun=std::bind(&Server::newConnection,this,std::placeholders::_1);//??????????????????????????????
    acceptor->setNewConnectionCallback(fun);

    int size=static_cast<int>(std::thread::hardware_concurrency());
    threadPool =new ThreadPool(size);
    for (int i=0; i<size; ++i) {
        sub_reactors.push_back(new EventLoop());
    }

    for (int i=0; i<size; ++i) {
        std::function<void()> sub_loop=std::bind(&EventLoop::loop,sub_reactors[i]);
        threadPool->add(std::move(sub_loop));
    }

}

Server::~Server()
{
    delete acceptor;
    delete threadPool;
}

void Server::newConnection(Socket *sock){
    // Connection *conn=new Connection(loop,sock);
    // function<void(Socket*)> fun=std::bind(&Server::deleteConnection,this,std::placeholders::_1);
    // conn->setDeleteConnectionCallback(fun);
    // connections[sock->getFd()]=conn;
    errif(sock->getFd()==-1, "new connection error");
    uint64_t random=sock->getFd()%sub_reactors.size();
    Connection *conn=new Connection(sub_reactors[random],sock);
    function<void(Socket*)> fn=std::bind(&Server::deleteConnection,this,std::placeholders::_1);
    conn->setDeleteConnectionCallback(fn);
    conn->setOnConnectCallback(on_connect_callback_);
    connections[sock->getFd()]=conn;
}

void Server::deleteConnection(Socket *sock){
    auto it=connections.find(sock->getFd());
    if (it!=connections.end()) {
        Connection *conn=connections[sock->getFd()];
        connections.erase(sock->getFd());
        delete conn;
        // conn=nullptr;
    }
}

void Server::OnConnect(std::function<void(Connection *)>fn){
    on_connect_callback_=fn;
}

//????    void handleReadEvent(int);//????实现呢？？
// void Server::handleReadEvent(int sockfd){
//     char buf[READ_BUFFER];
//     while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
//         bzero(&buf, sizeof(buf));
//         ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
//         if(bytes_read > 0){
//             printf("message from client fd %d: %s\n", sockfd, buf);
//             write(sockfd, buf, sizeof(buf));
//         } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
//             printf("continue reading");
//             continue;
//         } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
//             printf("finish reading once, errno: %d\n", errno);
//             break;
//         } else if(bytes_read == 0){  //EOF，客户端断开连接
//             printf("EOF, client fd %d disconnected\n", sockfd);
//             close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
//             break;
//         }
//     }
// }