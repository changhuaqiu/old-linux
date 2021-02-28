//
//  Channel.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/14.
//

#include "Channel.hpp"
#include "EventLoop.hpp"
#include "../base/Timestamp.hpp"

#include  <poll.h>
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop*loop,int fd)
    :loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(-1)
{
    
}

Channel::~Channel()
{
    assert(!eventHandling_);
}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::handleEvent(Timestamp reveiveTime)
{
    eventHandling_ = true;
    
    if(revents_ & POLLNVAL){
        
    }
    
    if((revents_ & POLLHUP) && !(revents_ & POLLIN))
    {
        if(closeCallback_) closeCallback_();
    }
    
    
    if(revents_ & (POLLERR | POLLNVAL)){
        if(errorCallback_)errorCallback_();
    }
    
    if(revents_ & ( POLLIN | POLLPRI )){
        if(readCallback_)readCallback_(reveiveTime);
    }
    if(revents_ & POLLOUT){
        if(writeCallback_)writeCallback_();
    }
    
    eventHandling_ = false;
}


void Channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ = obj;
    tied_ = true;
}

void Channel::remove()
{
  assert(isNoneEvent());
  loop_->removeChannel(this);
}
