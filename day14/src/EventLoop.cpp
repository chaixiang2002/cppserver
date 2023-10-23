#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"
EventLoop::EventLoop():quit(false)
{
    ep=new Epoll();
    // threadPool=new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete ep;
}

void EventLoop::loop(){
    while (!quit)
    {
        std::vector<Channel*> chs=ep->poll();
        for(auto &t:chs){
            t->handleEvent();
        }
    }
    
}
void EventLoop::updateChannel(Channel* ch){
    ep->updateChannel(ch);
}

// void EventLoop::addThread(std::function<void()> fun){
//     threadPool->add(fun);
// }