#include "Connection.h"
#include "Channel.h"
#include "Socket.h"
#include <string.h>
#include <unistd.h>
#include "util.h"

// #define READ_BUFFER 1024
using std::function;
Connection::Connection(EventLoop *_loop,Socket *_sock):loop(_loop),sock(_sock),inBuffer(new std::string())
{
    channel=new Channel(loop,sock->getFd());
    function<void()> fun=std::bind(&Connection::echo,this,sock->getFd());
    channel->setCallBack(fun);
    channel->enableReading();
    readBuffer=new Buffer;
}

Connection::~Connection()
{
    delete channel;
    delete sock;
}
void Connection::echo(int sockfd){
    char buf[1024];//这个buf大小无所谓了
    while (true)
    {
        bzero(&buf,sizeof(buf));
        ssize_t bytes_read=read(sockfd,buf,sizeof(buf));
        if(bytes_read>0){
            // printf("message from client fd %d: %s\n", sockfd, buf);
            // write(sockfd,buf,sizeof(buf));
            readBuffer->append(buf,bytes_read);
        }else if (bytes_read==-1 && errno==EINTR){//客户端正常中断、继续读取
            printf("continue reading");
            continue;
        }else if (bytes_read==-1 && (errno==EAGAIN || errno==EWOULDBLOCK ))//非阻塞IO，这个条件表示数据全部读取完毕
        {
            printf("finish reading once\n");
            printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
            errif(write(sockfd,readBuffer->c_str(),readBuffer->size())==-1,"sock write error");
            readBuffer->clear();
            break;
        }else if (bytes_read==0)//EOF，客户端断开连接
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            deleteConnectionCallback(sock);
            break;

        }
    }
    
}
void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> fun){
    deleteConnectionCallback=fun;
}

