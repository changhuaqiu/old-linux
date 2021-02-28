//
//  InetAddress.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/15.
//

#ifndef InetAddress_hpp
#define InetAddress_hpp

#include <stdio.h>
#include "../base/copyable.hpp"
#include <netinet/in.h>
#include <string>
#include <cstdint>


class InetAddress : public copyable
{

public:
  /// Constructs an endpoint with given port number.
  /// Mostly used in TcpServer listening.
  explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

  /// Constructs an endpoint with given ip and port.
  /// @c ip should be "1.2.3.4"
  InetAddress(const std::string& ip, uint16_t port, bool ipv6 = false);

  
  explicit InetAddress(const struct sockaddr_in& addr)
    : addr_(addr)
  { }


  sa_family_t family() const { return addr_.sin_family; }
  std::string toIp() const;
  std::string toIpPort() const;
  uint16_t toPort() const;

    
    const struct sockaddr* getSockAddr() const
    {
        return reinterpret_cast<const sockaddr*>(&addr_);
    }
    
     struct sockaddr* getSockAddr()
    {
        return reinterpret_cast<sockaddr*>(&addr_);
    }
  // default copy/assignment are Okay

 private:
  
    struct sockaddr_in addr_;
};
#endif
