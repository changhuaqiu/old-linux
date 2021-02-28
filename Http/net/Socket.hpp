//
//  Socket.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/15.
//

#ifndef Socket_hpp
#define Socket_hpp

#include <stdio.h>

#include "../base/noncopyable.hpp"
#include "InetAddress.hpp"
#include <netinet/tcp.h>
struct tcp_info;

class Socket : noncopyable
{
public:
    
    explicit Socket(int sockfd)
        : sockfd_(sockfd)
    {}
    
    ~Socket();
    
    int fd() const {return sockfd_;}
    
    bool getTcpInfo(struct tcp_info*)const;
    bool getTcpInfoString(char*buf,int len)const;
    
    void bindAddress(const InetAddress& localaddr);
    
    void listen();
    
    int accept(InetAddress* peeraddr);
    
    void shundownWrite();
    
    void setReuseAddr(bool on);
    
    void setReusePort(bool on);
    
    void setKeepAlive(bool on);
    
private:
    
    const int sockfd_;
};

#endif /* Socket_hpp */
