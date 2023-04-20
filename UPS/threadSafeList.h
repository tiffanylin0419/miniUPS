#include <list>
#include <mutex>

class ThreadSafeList {
 private:
  std::list<int> myList;
  std::mutex list_mutex;
  
 public:
  ThreadSafeList(){}

  void add(int seq){
    std::unique_lock<std::mutex> lock(list_mutex);
    myList.push_back(seq);
  }

  int getAndRemove(int seq){
    std::unique_lock<std::mutex> lock(list_mutex);
    int popped = myList.front();
    myList.pop_front();
    return popped;
  }

  ~ThreadSafeList(){}
};