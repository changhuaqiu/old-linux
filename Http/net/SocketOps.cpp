//
//  SocketOps.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/15.
//

#include "SocketOps.hpp"
#include "../base/log.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>  // snprintf
#include <sys/socket.h>
#include <sys/uio.h>  // readv
#include <unistd.h>
#include <netdb.h>
#include <cassert>
#include <arpa/inet.h>


void setNonBlockAndCloseOnExec(int sockfd)
{
    int flags = ::fcntl(sockfd, F_GETFL,0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd,F_SETFL,flags);
    flags = ::fcntl(sockfd,F_GETFD,0);
    flags |= FD_CLOEXEC;
    ret = ::fcntl(sockfd,F_SETFD,flags);
}

int sockets::CreateDefaultSocket(sa_family_t family)
{
    return socket(family,SOCK_STREAM ,0);
    
}

int sockets::CreateNonblockSocket(sa_family_t family)
{
    int sockfd = ::socket(family,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
    {
        
    }
    setNonBlockAndCloseOnExec(sockfd);
    return sockfd;
}

int sockets::Htons(int port)
{
    return htons(port);
}

void sockets::Bind(int sockfd, const sockaddr& address)
{
    int ret = bind(sockfd, &address, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
    assert(ret != -1);
}

int sockets::Connect(int sockfd, const sockaddr& address)
{
    return connect(sockfd, &address, static_cast<socklen_t>(sizeof address));
}

void sockets::Listen(int sockfd)
{
    ::listen(sockfd, 5);
}

void sockets::SetReusePort(int sockfd)
{
#ifndef _WIN32
    int on = 1;
    int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on,
        static_cast<socklen_t>(sizeof on));
    assert(ret != -1);
#endif
}

void sockets::SetReuseAddr(int sockfd)
{
#ifdef _WIN32
    char on = 1;
#else
    int on = 1;
#endif
    int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on,
        static_cast<socklen_t>(sizeof on));
    assert(ret != -1);
}

int sockets::Accept(int sockfd, struct sockaddr* address)
{

    socklen_t len = INET_ADDRSTRLEN;
    int fd = accept(sockfd, address, &len);
    setNonBlockAndCloseOnExec(fd);
    assert(fd >= 0);

    return fd;
}

void sockets::Close(int sockfd)
{
    ::close(sockfd);
}

ssize_t sockets::Send(int sockfd, const char* data, size_t length)
{
    return send(sockfd, data, length, 0);
}

void sockets::ShutdownWrite(int sockfd)
{
    ::shutdown(sockfd, SHUT_WR);
}

sockaddr_in sockets::GetPeerAddr(int sockfd)
{
    struct sockaddr_in peer_addr;
    socklen_t len = static_cast<socklen_t>(sizeof peer_addr);
    getpeername(sockfd, (sockaddr*)&peer_addr, &len);
    return peer_addr;
}

int sockets::InetPton(int af, const char* from, void* to)
{
    return ::inet_pton(af, from, to);
}

bool sockets::NameToAddr4(const std::string& name, in_addr* addr)
{
    if (InetPton(AF_INET, name.c_str(), addr) == 0)
    {
        hostent* host = gethostbyname(name.c_str());
        if (host)
        {
            addr->s_addr = *((uint32_t*)host->h_addr);
        }
        else
        {
           
            return false;
        }
    }
    return true;
}

void sockets::toIpPort(char*buf,size_t size,const struct sockaddr*addr)
{
    toIp(buf,size,addr);
    size_t end = ::strlen(buf);
    const struct sockaddr_in*addr4 = reinterpret_cast<const sockaddr_in*>(addr);
    uint16_t port = ntohs(addr4->sin_port); 
    assert(size > end);
    snprintf(buf+end, size-end, ":%u", port);
}

void sockets::toIp(char *buf, size_t size, const struct sockaddr *addr)
{
    assert(size >= INET_ADDRSTRLEN);
    
    const struct sockaddr_in*addr4 = reinterpret_cast<const sockaddr_in*>(addr);
    ::inet_ntop(AF_INET,&addr4->sin_addr,buf,static_cast<socklen_t>(size));
    
}

struct sockaddr_in sockets::getLocalAddr(int sockfd)
{
    struct sockaddr_in localaddr;
    memset(&localaddr, 0,sizeof localaddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    if (::getsockname(sockfd, reinterpret_cast<struct sockaddr*>(&localaddr), &addrlen) < 0)
    {
        
    }
      return localaddr;
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
    return ::read(sockfd, buf, count);
}
