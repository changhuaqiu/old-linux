//
//  noncopyable.hpp
//  qiuServer
//
//  Created by 邱昌华 on 2021/2/15.
//

#ifndef noncopyable_hpp
#define noncopyable_hpp

#include <stdio.h>

class noncopyable
{
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

#endif /* noncopyable_hpp */
