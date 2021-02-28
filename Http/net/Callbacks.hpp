//
//  Callbacks.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/16.
//

#ifndef Callbacks_hpp
#define Callbacks_hpp

#include <stdio.h>
#include "../base/Timestamp.hpp"
#include "../base/buffer.hpp"
#include <functional>
#include <memory>
class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void (const TcpConnectionPtr&, size_t)> HighWaterMarkCallback;

// the data has been read to (buf, len)
typedef std::function<void (const TcpConnectionPtr&,
                            Buffer*buf,
                            Timestamp)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            CloseCallback receiveTime);

#endif /* Callbacks_hpp */
