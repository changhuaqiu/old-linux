//
//  InetAddress.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/15.
//

#include "InetAddress.hpp"
#include "SocketOps.hpp"

InetAddress::InetAddress(uint16_t port, bool loopbackOnly,bool ipv6)
{
    memset(&addr_,0,sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = sockets::Htons(port);
    
}

InetAddress::InetAddress(const std::string& ip,uint16_t port,bool ipv6)
    : InetAddress(port,ipv6)
{
    sockets::NameToAddr4(ip, &addr_.sin_addr);
}



std::string InetAddress::toIp() const
{
    char buf[64] = "";
    sockets::toIp(buf, sizeof buf, getSockAddr());
    return buf;
}

std::string InetAddress::toIpPort() const
{
    char buf [64] = "";
    sockets::toIpPort(buf, sizeof buf, getSockAddr());
    return buf;
}

uint16_t InetAddress::toPort() const
{
    return 0;
}
