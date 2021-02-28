//
//  EventLoopThread.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/19.
//

#include "EventLoopThread.hpp"

#include "EventLoop.hpp"

EventLoopThread::EventLoopThread(const ThreadInitCallback&cb,
                                 const std::string& name)
    :loop_(NULL),
     exiting_(false),
     thread_(std::bind(&EventLoopThread::threadFunc,this)),
     mutex_(),
     callback_(cb)
{}

EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if(loop_ != NULL)
    {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop()
{
    
    thread_.detach();
    
    EventLoop* loop = NULL;
    {
        std::unique_lock<std::mutex>lock(mutex_);
        while(loop_ == NULL)
        {
            cond_.wait(lock);
        }
        loop = loop_;
    }
    return loop;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    
    if(callback_)
    {
        callback_(&loop);
    }
    {
        std::lock_guard<std::mutex>lock(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }
    loop.loop();
    std::lock_guard<std::mutex>lock(mutex_);
    loop_ = NULL;
}

