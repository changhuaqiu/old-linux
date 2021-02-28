//
//  EventLoopThread.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/19.
//

#ifndef EventLoopThread_hpp
#define EventLoopThread_hpp


#include "EventLoop.hpp"
#include "../base/noncopyable.hpp"

#include <stdio.h>
#include <thread>
#include <mutex>
#include <condition_variable>

class EventLoop;

class EventLoopThread : noncopyable
{
public:
    
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    
    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                    const std::string& name = std::string());
    
    ~EventLoopThread();
    EventLoop* startLoop();
    
private:
    
    void threadFunc();
    
    EventLoop* loop_;
    bool exiting_;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    ThreadInitCallback callback_;
};
#endif /* EventLoopThread_hpp */
