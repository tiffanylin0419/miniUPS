#include <map>
#include <mutex>

template<typename T>
class ThreadSafeMap {
 private:
  std::map<int, T> myMap;
  std::mutex map_mutex;
  
 public:
  ThreadSafeMap(){}

  void add(int seq, T command){
    std::unique_lock<std::mutex> lock(map_mutex);
    myMap.insert({seq, command});
  }

  void remove(int seq){
    std::unique_lock<std::mutex> lock(map_mutex);
    myMap.erase(seq);
  }

  ~ThreadSafeMap(){}
};