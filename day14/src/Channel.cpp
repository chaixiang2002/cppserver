#include "Channel.h"
// #include "Epoll.h"
#include "EventLoop.h"
#include <sys/epoll.h>
#include <unistd.h>
Channel::Channel(EventLoop *_loop,int _fd)
    :loop(_loop),fd(_fd),listen_events_(0),ready_events_(0),inEpoll(false){

}


Channel::~Channel()
{
    if(fd!=-1){
        close(fd);
        fd=-1;
    }
}

void Channel::handleEvent(){
    if(ready_events_ & (EPOLLIN | EPOLLPRI)){
        read_callback();
    }
    if (ready_events_ & (EPOLLOUT)) {
        write_callback();
    }
}
void Channel::enableReading(){
    listen_events_ |= EPOLLIN|EPOLLET;
    loop->updateChannel(this);
}

void Channel::UseET(){
    listen_events_ |= EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd(){
    return fd;  
}
uint32_t Channel::getListenEvents(){
    return listen_events_;
}
uint32_t Channel::getReadyEvents(){
    return ready_events_;
}
bool Channel::getInEpoll(){
    return inEpoll;
}

//

void Channel::setInEpoll(bool in){
    inEpoll=in;
}
void Channel::setReadyEvents(uint32_t _ev){
    ready_events_ = _ev;
}

void Channel::setReadCallback(std::function<void()> const &callback){
    read_callback=callback;
}
