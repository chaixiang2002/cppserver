#include "Epoll.h"
#include "util.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include "Channel.h"

#define MAX_EVENTS 1000

Epoll::Epoll()
{
    epfd=epoll_create1(0);
    errif(epfd==-1,"epoll create error");
    events=new epoll_event[MAX_EVENTS];
    bzero(events,sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if(epfd!=-1){
        close(epfd);
        epfd=-1;
    }
    delete [] events;
}
  
// void Epoll::addFd(int fd,uint32_t op){
//     struct epoll_event ev;
//     bzero(&ev,sizeof(ev));
//     ev.data.fd=fd;
//     ev.events=op;
//     errif(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev)==-1,"epoll add event error");
// }

using std::vector;
/**
 * day03时 疑问activeEvents????为什么要复制一遍？？
 * 
 * day04引入Channel类，不需要在复制一遍events[]
 * 现在一个事件对应一个 Channel，ch指向事件的指针即可。
*/
std::vector<Channel*> Epoll::poll(int timeout){
    vector<Channel*> activeEvents;
    int nfds=epoll_wait(epfd,events,MAX_EVENTS,timeout);
    errif(nfds==-1,"epoll wait error");
    for (int i = 0; i < nfds; i++)
    {
        Channel *ch=(Channel*)events[i].data.ptr;
        ch->setReadyEvents(events[i].events);
        activeEvents.push_back(ch);
    }
    return activeEvents; 
}

void Epoll::updateChannel(Channel *channel){
    int fd=channel->getFd();
    struct epoll_event ev;
    ev.data.ptr=channel;
    ev.events=channel->getListenEvents();
    if(!channel->getInEpoll()){
        errif(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev)==-1,"epoll add error");
        channel->setInEpoll();
    }else{
        errif(epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev)==-1,"epoll mod error");
    }
}

void Epoll::deleteChannel(Channel *ch){
    int fd=ch->getFd();
    errif(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr)==-1, "epoll delete error");
}
