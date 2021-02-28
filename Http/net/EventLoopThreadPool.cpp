//
//  EventLoopThreadPool.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/19.
//

#include "EventLoopThreadPool.hpp"

#include "EventLoop.hpp"
#include "EventLoopThread.hpp"


EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop,const std::string & nameArg)
    :baseLoop_(baseLoop),
     name_(nameArg),
     started_(false),
     numThreads_(0),
     next_(0)
{}

EventLoopThreadPool::~EventLoopThreadPool()
{}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
    started_ = true;
    
    for(int i = 0; i < numThreads_; ++i)
    {
        char buf[name_.size() + 32];
        sprintf(buf, "%s%d",name_.c_str(),i);
        EventLoopThread* t  = new EventLoopThread(cb,buf);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->startLoop());
    }
        if(numThreads_ == 0 && cb)
        {
            cb(baseLoop_);
        }
        
    
    
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    assert(started_);
    EventLoop* loop = baseLoop_;
    
    if(!loops_.empty())
    {
        loop = loops_[next_];
        ++next_;
        if(static_cast<size_t>(next_) >= loops_.size())
        {
            next_ = 0;
        }
    }
    return loop;
}



