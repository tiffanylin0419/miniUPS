#include "threadSafeQueue.h"

template<typename T>
  void ThreadSafeQueue<T>::add(int seq, T command){
    std::unique_lock<std::mutex> lock(map_mutex);
    myList.push(std::make_pair(seq, command));
    cv.notify_one();
  }

template<typename T>
  void ThreadSafeQueue<T>::remove(int seq){
    std::unique_lock<std::mutex> lock(map_mutex);
    for(int i=0;i<myList.size();i++){
      std::pair<int, T> c=myList.front();
      myList.pop();
      if(c.first!=seq){
        myList.push(c);
      }
    }
  }

template<typename T>
  T ThreadSafeQueue<T>::getOne(){
    std::unique_lock<std::mutex> lock(map_mutex);
    while (myList.empty()) {
      cv.wait(lock);
    }
    std::pair<int, T> c=myList.front();
    myList.pop();
    myList.push(c);
    return c.second;
  }

template<typename T>
  T ThreadSafeQueue<T>::getAndRemove(){
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


