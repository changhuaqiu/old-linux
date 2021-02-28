//
//  TcpConnection.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/16.
//

#ifndef TcpConnection_hpp
#define TcpConnection_hpp

#include <stdio.h>
#include "../base/noncopyable.hpp"
#include <boost/any.hpp>
#include <memory>
#include "EventLoop.hpp"
#include "Socket.hpp"
#include "Channel.hpp"
#include "Callbacks.hpp"
#include "InetAddress.hpp"
#include "../base/buffer.hpp"

class TcpConnection : noncopyable,
public std::enable_shared_from_this<TcpConnection>
{
public:
    
    TcpConnection(EventLoop*loop,std::string& name,int sockfd,const InetAddress&localAddr,const InetAddress&peerAddr);
        
    
        
    
    
    
    void setConnectionCallback(const ConnectionCallback& cb)
      { connectionCallback_ = cb; }

      void setMessageCallback(const MessageCallback& cb)
      { messageCallback_ = cb; }
    
    void setCloseCallback(const CloseCallback& cb)
    { closeCallback_ = cb; }
    
    bool connected(){ return state_ ==kConnected; }
    
    std::string name(){ return name_;}
    
    InetAddress& peerAddress() { return peerAddr_;}
    
    InetAddress& localAddress() { return localAddr_;}
    
    void connectEstablished();
    
    void connectDestoryed();
    
    const std::string name()const{return name_;}
    
private:
    
    enum StateE { kConnecting, kConnected,kDisconnected,kDisconnecting,};
    
    void setState(StateE s){state_ = s;}
    
    void handleRead(Timestamp receiveTime);
    
    void handleWrite();
    
    void handleClose();
    
    void handleError();
    
    void sendInLoop(const std::string& message);
    
    void shutdownInLoop();
    
    
    EventLoop*loop_;
    std::string name_;
    StateE state_;
    
    std::unique_ptr<Socket>socket_;
    std::unique_ptr<Channel>channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback    messageCallback_;
    CloseCallback      closeCallback_;
    
    Buffer inputBuffer_;
    Buffer outputBuffer_;
};
#endif /* TcpConnection_hpp */
