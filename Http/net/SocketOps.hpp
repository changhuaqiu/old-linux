//
//  SocketOps.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/15.
//

#ifndef SocketOps_hpp
#define SocketOps_hpp

#include <stdio.h>
#include <arpa/inet.h>
#include <string>

namespace sockets
{

int CreateDefaultSocket(sa_family_t family);

int CreateNonblockSocket(sa_family_t family);

int Htons(int port);

void Bind(int sockfd, const sockaddr& address);

int Connect(int sockfd, const sockaddr& address);

void Listen(int sockfd);

void SetReusePort(int sockfd);

void SetReuseAddr(int sockfd);

int Accept(int sockfd, struct sockaddr* address);

ssize_t Send(int sockfd, const char* data, size_t length);

void Close(int sockfd);

void ShutdownWrite(int sockfd);

sockaddr_in GetPeerAddr(int sockfd);

int InetPton(int af, const char* from, void* to);

bool NameToAddr4(const std::string& name, in_addr* addr);

void toIpPort(char* buf, size_t size,
              const struct sockaddr* addr);
void toIp(char* buf, size_t size,
          const struct sockaddr* addr);

void NameToAddr6(const std::string& name, in6_addr* addr);

struct sockaddr_in getLocalAddr(int sockfd);

ssize_t read(int sockfd, void *buf, size_t count);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void *buf, size_t count);


}
#endif /* SocketOps_hpp */
