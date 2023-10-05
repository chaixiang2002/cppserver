#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include "Epoll.h"
using std::vector;

EventLoop::EventLoop():quit(false)
{
    ep=new Epoll();
}

EventLoop::~EventLoop()
{
    delete ep;
}

void EventLoop::loop(){
    while (!quit)
    {
        vector<Channel*> chs=ep->poll();
        for(auto &t:chs){
            t->handleEvent();
        }
    }
    
}
void EventLoop::updateChannel(Channel* ch){
    ep->updateChannel(ch);
}