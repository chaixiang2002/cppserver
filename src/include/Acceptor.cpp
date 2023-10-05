#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Server.h"
#include "Channel.h"

using std::function;

Acceptor::Acceptor(EventLoop *_loop):loop(_loop)
{
    sock=new Socket();
    addr=new InetAddress("127.0.0.1",8088);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();

    acceptChannel=new Channel(loop,sock->getFd());
    function<void()> fun=std::bind(&Acceptor::acceptConnection,this);// 绑定函数
    acceptChannel->setCallBack(fun);// 给serv_channel绑定函数
    acceptChannel->enableReading();// 让serv_channel可读：serv_channel.loop.epoll.add(serv_channel)
   
}

Acceptor::~Acceptor()
{
    delete sock;
    delete addr;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){
    newConnectCallback(sock);
}
void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> fun){
    newConnectCallback=fun;
}
