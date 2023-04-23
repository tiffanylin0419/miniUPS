

#include <set>
#include <mutex>
#include "threadSafeSet.h"


  void ThreadSafeSet::add(int seq){
    std::unique_lock<std::mutex> lock(set_mutex);
    mySet.insert(seq);
  }

  bool ThreadSafeSet::contains(int seq){
    std::unique_lock<std::mutex> lock(set_mutex);
    if(mySet.count(seq)==0){
        return false;
    }
    return true;
  }
