#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "../proto/ups.pb.h"
#include "../proto/ups-amazon.pb.h"

template<typename T>
class ThreadSafeQueue {
 private:
  std::queue<std::pair<int, T>> myList;
  std::mutex map_mutex;
  std::condition_variable cv;
  
 public:
  ThreadSafeQueue(){}

  void add(int seq, T command){
    std::unique_lock<std::mutex> lock(map_mutex);
    myList.push(std::make_pair(seq, command));
    cv.notify_one();
  }

  void remove(int seq){
    std::unique_lock<std::mutex> lock(map_mutex);
    for(int i=0;i<myList.size();i++){
      std::pair<int, T> c=myList.front();
      myList.pop();
      if(c.first!=seq){
        myList.push(c);
      }
    }
  }

  T getOne(){
    std::unique_lock<std::mutex> lock(map_mutex);
    while (myList.empty()) {
      cv.wait(lock);
    }
    std::pair<int, T> c=myList.front();
    myList.pop();
    myList.push(c);
    return c.second;
  }

  T getAndRemove(){
    std::unique_lock<std::mutex> lock(map_mutex);
    while (myList.empty()) {
      cv.wait(lock);
    }
    std::pair<int, T> c=myList.front();
    myList.pop();
    return c.second;
  }

  // int getCommandSeqNum(UCommands command){
  //   for(int i=0;i<command.pickups_size();i++){
  //       return command.pickups(i).seqnum();
  //   }
  //   for(int i=0;i<command.deliveries_size();i++){
  //       return command.deliveries(i).seqnum();
  //   }
  //   for(int i=0;i<command.queries_size();i++){
  //       return command.queries(i).seqnum();
  //   }
  //   return -1;
  // }

  ~ThreadSafeQueue(){}
};



#endif 