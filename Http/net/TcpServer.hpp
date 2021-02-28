//
//  TcpServer.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/16.
//

#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <stdio.h>
#include <map>  
#include "../base/noncopyable.cpp"
#include "InetAddress.hpp"
#include "Acceptor.hpp"
#include "Callbacks.hpp"
#include "TcpConnection.hpp"
#include "EventLoopThreadPool.hpp"
class Acceptor;
class EventLoop;


//TcpServer持有 Acceptor  设置acceptor 接入新连接 回调函数 newconnection()
//newconnection() 注册 connection对象
//每个对象都有关注的事件 专注于自己层面事件的实现。

class TcpServer : noncopyable
{
public:
    
   
    
    TcpServer(EventLoop* loop,const InetAddress & listenAddr,const std::string & nameArg);
    ~TcpServer();
    
    void start();
    
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb;}
    
    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb;}
    
    void removeConnection(const TcpConnectionPtr& conn);
    
    void removeConnectionInLoop(const TcpConnectionPtr& conn);
private:
    
    void newConnection(int sockfd,const InetAddress & peerAddr);
    
    typedef std::map<std::string,TcpConnectionPtr>ConnectionMap;
    
    EventLoop* loop_;
    
    
    const std::string name_;
    
    std::unique_ptr<Acceptor>acceptor_;
    std::unique_ptr<EventLoopThreadPool> threadPool_;
    
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback   closeCallback_;
    
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;
};
#endif /* TcpServer_hpp */
