//
//  TcpConnection.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/16.
//
#include <functional>

#include "TcpConnection.hpp"
#include "EventLoop.hpp"
#include "SocketOps.hpp"
#include "Channel.hpp"


TcpConnection::TcpConnection(EventLoop*loop,std::string&name,int sockfd,const InetAddress&localAddr,const InetAddress&peerAddr)
   :loop_(loop),
    name_(name),
    state_(kConnecting),
    socket_(new Socket(sockfd)),
    channel_(new Channel(loop, sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr)
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this,std::placeholders::_1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite,this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError,this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose,this));
    socket_->setKeepAlive(true);
}


void TcpConnection::handleRead(Timestamp reveiveTime)
{
    int saveErrno = 0;
    char buf[65536];
    ssize_t n = inputBuffer_.ReadFd(channel_->fd(), &saveErrno);
    if(n > 0){
        messageCallback_(shared_from_this(),&inputBuffer_,reveiveTime);
    }
    else if(n == 0)
    {
        handleClose();
    }
    else{
        handleError();
    }
}
void TcpConnection::handleClose()
{
    assert(state_ == kConnected);
    channel_->disableAll();
    closeCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
    
}

void TcpConnection::handleWrite()
{
    
}
void TcpConnection::connectEstablished()
{
    
      assert(state_ == kConnecting);
      setState(kConnected);
      channel_->tie(shared_from_this());
      channel_->enableReading();

      connectionCallback_(shared_from_this());
}


void TcpConnection::connectDestoryed()
{
    assert(state_ == kConnected);
    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());
    channel_->remove();
}
