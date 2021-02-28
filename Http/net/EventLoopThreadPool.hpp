//
//  EventLoopThreadPool.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/19.
//

#ifndef EventLoopThreadPool_hpp
#define EventLoopThreadPool_hpp

#include <stdio.h>

#include "../base/noncopyable.hpp"
#include "EventLoopThread.hpp"
#include "EventLoop.hpp"

class EventLoop;
class EventLoopThread;


class EventLoopThreadPool : noncopyable
{
public:
    
    typedef std::function<void(EventLoop*)>ThreadInitCallback;
    
    EventLoopThreadPool(EventLoop* baseLoop,const std::string & nameArg);
    ~EventLoopThreadPool();
    
    void setThreadNum(int numThreads) { numThreads_  = numThreads;}
    
    void start(const ThreadInitCallback& cb);
    EventLoop* getNextLoop();
    
private:
    
    EventLoop* baseLoop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>>threads_;
    std::vector<EventLoop*>loops_;
};

#endif /* EventLoopThreadPool_hpp */
