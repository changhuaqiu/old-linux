//
//  EventLoop.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/14.
//

#include "EventLoop.hpp"
#include "SocketOps.hpp"

#include <sys/eventfd.h>

EventLoop::EventLoop()
    : looping_(false),
     threadId_(getpid()),
    poller_(new Poller(this)),
    wakeupFd_(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
    wakeupChannel_(new Channel(this,wakeupFd_)),
    activeChannels_(NULL)
{
    wakeupChannel_->setWriteCallback(std::bind(&EventLoop::handleRead,this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    
}

void EventLoop::updateChannel(Channel* channel){
    assert(channel->ownerLoop() == this);
    
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
      poller_->removeChannel(channel);
    
}

void EventLoop::quit(){
    quit_ = true;
}

void EventLoop::loop()
{
    
    assert(!looping_);
    looping_ = true;
    quit_ = false;
    
    while(!quit_){
        activeChannels_.clear();
        
        poller_->poll(10,&activeChannels_);
        for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end();++it){
            (*it)->handleEvent(pollReturnTime_);
        }
        
        doPendingFunctors();
    }
    
    
}

void EventLoop::runInLoop(const Functor &cb)
{
    if(isInLoopThread())
    {
        cb();
    }else{
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const Functor &cb)
{
    std::lock_guard<std::mutex>lock(mutex_);
    pendingFunctors_.push_back(cb);
}


void EventLoop::doPendingFunctors()
{
    std::vector<Functor>functors;
    callingPendingFunctors_ = true;
    {
        std::lock_guard<std::mutex>lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for(auto&func:pendingFunctors_)
    {
        func();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::abortNotInLoopThread()
{
  
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = sockets::write(wakeupFd_,&one,sizeof one);
    if(n != sizeof one)
    {
        
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
    if(n != sizeof one)
    {
        
    }
}
