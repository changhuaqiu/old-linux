//
//  Socket.cpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/15.
//

#include "Socket.hpp"
#include "../base/log.hpp"
#include "SocketOps.hpp"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <sys/socket.h>

struct tcp_info;

Socket::~Socket()
{
    sockets::Close(sockfd_);
}


bool Socket::getTcpInfo(struct tcp_info* tcpi) const
{
    return false;
}

//bool Socket::getTcpInfoString(char* buf, int len) const
//{
  //struct tcp_info tcpi;
  //bool ok = getTcpInfo(&tcpi);
    
 // if (ok)
  //{
    //snprintf(buf, len, "unrecovered=%u "
      //       "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
        //     "lost=%u retrans=%u rtt=%u rttvar=%u "
          //   "sshthresh=%u cwnd=%u total_retrans=%u",
            // tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
             //tcpi.tcpi_rto,          // Retransmit timeout in usec
             //tcpi.tcpi_ato,          // Predicted tick of soft clock in usec
             //tcpi.tcpi_snd_mss,
           //  tcpi.tcpi_rcv_mss,
         //    tcpi.tcpi_lost,         // Lost packets
       //      tcpi.tcpi_retrans,      // Retransmitted packets out
     //        tcpi.tcpi_rtt,          // Smoothed round trip time in usec
   ////          tcpi.tcpi_rttvar,       // Medium deviation
   //          tcpi.tcpi_snd_ssthresh,
 //            tcpi.tcpi_snd_cwnd,
 //            tcpi.tcpi_total_retrans);  // Total retransmits for entire connection
//  }
///  return ok;
//}

void Socket::bindAddress(const InetAddress& addr)
{
  sockets::Bind(sockfd_, *addr.getSockAddr());
}

void Socket::listen()
{
  sockets::Listen(sockfd_);
}

int Socket::accept(InetAddress* peeraddr)
{
  struct sockaddr_in addr;
  memset(&addr, 0,sizeof addr);
  int connfd = sockets::Accept(sockfd_,peeraddr->getSockAddr());
  return connfd;
}

//void Socket::shutdownWrite()
//{
 // sockets::shutdownWrite(sockfd_);
//}



void Socket::setReuseAddr(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
  if (ret < 0 && on)
  {
   
  }
#else
  if (on)
  {
    LOG_ERROR << "SO_REUSEPORT is not supported.";
  }
#endif
}

void Socket::setKeepAlive(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}
