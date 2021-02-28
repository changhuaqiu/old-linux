//
//  EventLoop.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/14.
//

#ifndef EventLoop_hpp
#define EventLoop_hpp

#include <stdio.h>
#include <boost/any.hpp>
#include <functional>
#include <mutex>

#include "Channel.hpp"
#include "Poller.hpp"
#include "../base/noncopyable.hpp"

class Channel;
class Poller;

class EventLoop    :noncopyable
{
    
public:
    
    EventLoop();
    ~EventLoop();
    
    typedef std::function<void()> Functor;
    
    void loop();
    
    void updateChannel(Channel* channel);
    
    void removeChannel(Channel* channel);
    
    void quit();
    
    void queueInLoop(const Functor& cb);
    
    
    void runInLoop(const Functor& cb);
    
    void assertInLoopThread()
    {
        if(!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }
    
    bool isInLoopThread() const { return threadId_ == getpid();}
    
    void wakeup();
    
    void handleRead();
    
private:
    
    typedef std::vector<Channel*> ChannelList;
    
    void abortNotInLoopThread();
    
    const pid_t threadId_;
    
    void handleRead();
    
    void doPendingFunctors();
    
    bool quit_; //结束标志
    
    std::unique_ptr<Poller>poller_; //
    
    ChannelList activeChannels_;
    
    bool looping_;  //循环标志
    
    std::vector<Functor>pendingFunctors_;
    
    std::mutex mutex_;
    
    int wakeupFd_;
    
    std::unique_ptr<Channel>wakeupChannel_;
    
    bool callingPendingFunctors_;
    
    Timestamp pollReturnTime_;
    
    
    
    
};
#endif /* EventLoop_hpp */
