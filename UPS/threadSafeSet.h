#ifndef THREAD_SAFE_SET_H
#define THREAD_SAFE_SET_H

#include <set>
#include <mutex>

class ThreadSafeSet {
 private:
  std::set<int> mySet;
  std::mutex set_mutex;
  
 public:
  ThreadSafeSet(){}
  void add(int seq);
  bool contains(int seq);
  ~ThreadSafeSet(){}
};

#endif // THREAD_SAFE_SET_H