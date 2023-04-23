#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "proto/ups.pb.h"
#include "proto/ups-amazon.pb.h"

template<typename T>
class ThreadSafeQueue {
 private:
  std::queue<std::pair<int, T>> myList;
  std::mutex map_mutex;
  std::condition_variable cv;
  
 public:
  ThreadSafeQueue(){}

  void add(int seq, T command);
  void remove(int seq);
  T getOne();
  T getAndRemove();
  ~ThreadSafeQueue(){}
};

#endif //THREAD_SAFE_QUEUE_H