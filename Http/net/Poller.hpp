//
//  Poller.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/14.
//

#ifndef Poller_hpp
#define Poller_hpp

#include <stdio.h>
#include <boost/any.hpp>
#include <poll.h>
#include <map>
#include <vector>

#include "../base/Timestamp.hpp"
#include "../base/noncopyable.hpp"
#include "EventLoop.hpp"
#include "Channel.hpp"


struct pollfd;

class Channel;

class Poller  : noncopyable
{
public:
    
    typedef std::vector<Channel*>ChannelList;
    
    Poller(EventLoop * loop);
    ~Poller();
    
    Timestamp poll(int timeoutMs,ChannelList* activeChannels);
    
    void updateChannel(Channel*channel);
    
    void removeChannel(Channel* channel);
   
    
private:
    
    void fillActiveChannels(int numEvents,ChannelList*activeChannels)const;
    
    typedef std::vector<struct pollfd> PollFdList ;
    
    typedef std::map<int,Channel*>ChannelMap;
    
    EventLoop* ownerLoop_;  //主loop
    PollFdList pollfds_;  //描述符表
    ChannelMap channels_; //channel map映射表
    
};
#endif /* Poller_hpp */
