//
//  Channel.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/14.
//
//channel 负责一个文件描述符的IO事件分发。
#ifndef Channel_hpp
#define Channel_hpp

#include <stdio.h>
#include <functional>
#include <boost/any.hpp>

#include "../base/Timestamp.hpp"
#include "../base/noncopyable.hpp"
class EventLoop;


//使用描述符 但是不拥有描述符
class Channel :noncopyable
{
public:
    
    typedef std::function<void()> EventCallback;
    
    typedef  std::function<void(Timestamp)> ReadEventCallback;
    
    Channel(EventLoop*loop , int fd);
    
    ~Channel();
    void handleEvent(Timestamp reveieTime);
    
    void setReadCallback(const ReadEventCallback & cb)
    { readCallback_ = cb;}
    
    void setWriteCallback(const EventCallback & wb)
    { writeCallback_ = wb;}
    
    void setErrorCallback(const EventCallback& eb)
    {errorCallback_ = eb;}
    
    void setCloseCallback(const EventCallback& cb)
    {closeCallback_ = cb;}
    
    int fd()const {return fd_;}
    
    int events()const{return events_;}
    void set_revents(int reve){revents_ = reve;}
    bool isNoneEvent()const{return events_ == kNoneEvent;}
    
    void enableReading(){events_ |= kReadEvent;  update();}
    
    void enableWriting(){events_ |= kWriteEvent; update();}
    
    void disableWriting(){ events_ &= ~kWriteEvent; update();}
    
    void disableAll(){events_ |= kNoneEvent; update();}
    
    bool isWriting() const {return events_ & kWriteEvent; }
    int index(){return index_;} //pollfd数组中的位置
    
    void set_index(int idx){index_ = idx;}  //设置下标
    
    EventLoop* ownerLoop() { return loop_; }
    
    void tie(const std::shared_ptr<void>&);
    
    void remove();
    
private:
    
    void update();
    
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    
    EventLoop * loop_;
    const int fd_;
    
    int events_;
    int revents_;
    int index_;
    
    bool eventHandling_;
    
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback closeCallback_;
    
    std::weak_ptr<void> tie_;
    bool tied_;
};

#endif /* Channel_hpp */
