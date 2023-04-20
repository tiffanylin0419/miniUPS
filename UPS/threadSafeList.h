#include <list>
#include <mutex>

template<typename T>
class ThreadSafeList {
 private:
  std::list<T> myList;
  std::mutex list_mutex;
  
 public:
  ThreadSafeList(){}

  void add(int seq, T command){
    std::unique_lock<std::mutex> lock(list_mutex);
    myList.push_back(command);
  }

  T getAndRemove(int seq){
    std::unique_lock<std::mutex> lock(list_mutex);
    int popped = myList.front();
    myList.pop_front();
    return popped;
  }

  ~ThreadSafeList(){}
};