//
//  Acceptor.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/15.
//

#ifndef Acceptor_hpp
#define Acceptor_hpp

#include <stdio.h>
#include "../base/noncopyable.hpp"
#include "EventLoop.hpp"
#include "InetAddress.hpp"
#include "SocketOps.hpp"
#include "Socket.hpp"

//acceptor 关注 新连接事件  创建 acceptor 设置回调。acceptor.listen 注册channel。回调函数为处理连接事件
// 创建新的connection对象。
class Acceptor : noncopyable
{
public:
    
    typedef std::function<void(int sockfd,const InetAddress&)> NewConnectionCallback;
    
    Acceptor(EventLoop*loop,const InetAddress&listenAddr);
    
    void setNewConnectionCallback(const NewConnectionCallback&cb)
    {
        newConnectionCallback_ = cb;
    }
    
    bool listening()const{return listenning_;}
    
    void listen();
    
private:
    
    void handleRead();
    
    EventLoop* loop_;
    
    Socket acceptSocket_;
    
    Channel acceptChannel_;
    
    NewConnectionCallback newConnectionCallback_;
    
    bool listenning_;
    
};

#endif /* Acceptor_hpp */
