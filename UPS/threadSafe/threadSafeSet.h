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

  void add(int seq){
    std::unique_lock<std::mutex> lock(set_mutex);
    mySet.insert(seq);
  }

  bool contains(int seq){
    std::unique_lock<std::mutex> lock(set_mutex);
    if(mySet.count(seq)==0){
        return false;
    }
    return true;
  }

  ~ThreadSafeSet(){}
};


#endif 