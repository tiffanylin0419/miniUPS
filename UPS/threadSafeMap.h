#include <queue>
#include <mutex>
#include "proto/ups.pb.h"
#include "proto/ups-amazon.pb.h"

template<typename T>
class ThreadSafeMap {
 private:
  std::queue<T> myList;
  std::mutex map_mutex;
  
 public:
  ThreadSafeMap(){}

  void add(T command){
    std::unique_lock<std::mutex> lock(map_mutex);
    myList.push(command);
  }

  void remove(int seq){
    std::unique_lock<std::mutex> lock(map_mutex);
    for(int i=0;i<myList.size();i++){
      T c=myList.front();
      myList.pop();
      if(getCommandSeqNum(c)!=seq){
        myList.push(c);
      }
    }
  }

  T waitPop(){
    std::unique_lock<std::mutex> lock(map_mutex);
    T c=myList.front();
    myList.pop();
    myList.push(c);
    return c;
  }

  int getCommandSeqNum(T command){
    for(int i=0;i<command.pickups_size();i++){
        return command.pickups(i).seqnum();
    }
    for(int i=0;i<command.deliveries_size();i++){
        return command.deliveries(i).seqnum();
    }
    for(int i=0;i<command.queries_size();i++){
        return command.queries(i).seqnum();
    }
    return -1;
  }

  ~ThreadSafeMap(){}
};

