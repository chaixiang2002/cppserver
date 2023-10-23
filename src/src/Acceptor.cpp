#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Server.h"
#include "Channel.h"

using std::function;

Acceptor::Acceptor(EventLoop *_loop):loop(_loop)
{
    sock=new Socket();
    InetAddress *addr=new InetAddress("127.0.0.1",1234);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();

    acceptChannel=new Channel(loop,sock->getFd());
    function<void()> fun=std::bind(&Acceptor::acceptConnection,this);// 绑定函数
    acceptChannel->setCallBack(fun);// 给serv_channel绑定函数
    acceptChannel->enableReading();// 让serv_channel可读：serv_channel.loop.epoll.add(serv_channel)
    delete addr;
}

Acceptor::~Acceptor()
{
    delete acceptChannel;

    delete sock;
    // delete addr;
}

void Acceptor::acceptConnection(){
    // newConnectCallback(sock);
    InetAddress *clnt_addr=new InetAddress();
    Socket *clnt_sock=new Socket(sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    newConnectCallback(clnt_sock);
    delete clnt_addr;
}
void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> fun){
    newConnectCallback=fun;
}
