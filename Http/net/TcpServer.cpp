//
//  TcpServer.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/16.
//

#include <stdio.h>
#include "TcpConnection.hpp"
#include "TcpServer.hpp"


 TcpServer::TcpServer(EventLoop*loop,const InetAddress & listenAddr , const std::string & nameArg)
    :loop_(loop),
     name_(nameArg),
     acceptor_(new Acceptor(loop_,listenAddr)),
     nextConnId_(1)
     
{
    
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection,this,std::placeholders::_1,std::placeholders::_2));
}

TcpServer::~TcpServer()
{
    
}
void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr)
{
    char buf[32];
    snprintf(buf, sizeof buf,"#%d",nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;
    
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    EventLoop* ioLoop = threadPool_->getNextLoop();
    TcpConnectionPtr conn(new TcpConnection(ioLoop,connName,sockfd,localAddr,peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection,this,std::placeholders::_1));
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished,conn));
    
}

void TcpServer::start()
{
    acceptor_->listen();
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    loop_->runInLoop(std::bind(&::TcpServer::removeConnection,this,conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
    size_t n = connections_.erase(conn->name());
    assert( n == 1);
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestoryed,conn));
}


