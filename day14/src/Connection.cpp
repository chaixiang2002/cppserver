#include "Connection.h"
#include "Buffer.h"
#include "Channel.h"
#include "Macros.h"
#include "Socket.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstdio>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "util.h"

// #define READ_BUFFER 1024
using std::function;
Connection::Connection(EventLoop *_loop,Socket *_sock):loop(_loop),sock(_sock)
{
    // channel=new Channel(loop,sock->getFd());
    // function<void()> fun=std::bind(&Connection::echo,this,sock->getFd());
    // channel->setCallBack(fun);
    // channel->enableReading();
    // readBuffer=new Buffer;
    if (loop!=nullptr) {
        channel=new Channel(loop,sock->getFd());
        channel->enableReading();
        channel->UseET();
    }
    readBuffer=new Buffer();
    sendBuffer=new Buffer();
    state_=State::Connected;
}

Connection::~Connection()
{   
    if(loop!=nullptr){
        delete channel;
    }
    delete sock;
    delete readBuffer;
    delete sendBuffer;
}
void Connection::Read(){
    ASSERT(state_==State::Connected, "connection state is disconnected!");
    readBuffer->clear();
    if (sock->IsNonBlocking()) {
        ReadNonBlocking();
    }else {
        ReadBlocking();
    }
}

void Connection::Write(){
    ASSERT(state_==State::Connected, "connection state is disconnected!");
    if(sock->IsNonBlocking()){
        WriteNonBlocking();
    }else {
        WriteBlocking();
    }
    sendBuffer->clear();
}

void Connection::ReadNonBlocking(){
    int sockfd=sock->getFd();
    char buf[1024];
    while (true) {
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read=read(sockfd, buf, sizeof(buf));
        if(bytes_read>0){
            readBuffer->append(buf,bytes_read);
        }else if (bytes_read==-1 &&errno==EINTR) {
            printf("continue reading\n");
            continue;
        }else if (bytes_read && ((errno==EAGAIN)||(errno==EWOULDBLOCK))) {
            break;
        }else if (bytes_read == 0) {  // EOF，客户端断开连接
            printf("read EOF, client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            break;
        }else {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            break;
        }
    }
}

void Connection::WriteNonBlocking(){
    int sockfd=sock->getFd();
    char buf[sendBuffer->size()];
    memcpy(buf, sendBuffer->ToStr(),sendBuffer->size());
    int data_size=sendBuffer->size();
    int data_left=data_size;
    while (data_left>0) {
        ssize_t bytes_write=write(sockfd, buf+data_size-data_left, data_left);
        if (bytes_write == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
         break;
        }
        if (bytes_write == -1) {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            break;
        }
        data_left-=bytes_write;
    }
}

void Connection::ReadBlocking(){
    int sockfd=sock->getFd();
    unsigned int rcv_size=0;
    socklen_t len=sizeof(rcv_size);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
        readBuffer->append(buf, bytes_read);
    } else if (bytes_read == 0) {
        printf("read EOF, blocking client fd %d disconnected\n", sockfd);
        state_ = State::Closed;
    } else if (bytes_read == -1) {
        printf("Other error on blocking client fd %d\n", sockfd);
        state_ = State::Closed;
    }
}

/**
 * @brief Never used by server, only for client
 *
 */
void Connection::WriteBlocking() {
  // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
  int sockfd = sock->getFd();
  ssize_t bytes_write = write(sockfd, sendBuffer->ToStr(), sendBuffer->size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
}

void Connection::close(){deleteConnectionCallback(sock);}

Connection::State Connection::GetState() { return state_; }
void Connection::SetSendBuffer(const char *str) { sendBuffer->setBuf(str); }
Buffer *Connection::GetReadBuffer() { return readBuffer; }
const char *Connection::ReadBuffer() { return readBuffer->ToStr(); }
Buffer *Connection::GetSendBuffer() { return sendBuffer; }
const char *Connection::SendBuffer() { return sendBuffer->ToStr(); }



void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> const &callback){
    deleteConnectionCallback=callback;
}

void Connection::setOnConnectCallback(std::function<void(Connection *)> const &callback){
    on_connect_callback_=callback;
    channel->setReadCallback([this]()->void{
        on_connect_callback_(this);
    });
}


void Connection::GetlineSendBuffer() { sendBuffer->getline(); }

Socket *Connection::GetSocket() { return sock; }
