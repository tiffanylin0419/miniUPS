#include "proto/ups.pb.h"
#include "connect_func.h"
#include "connect_world.cpp"


class Server {
 private:
  const char * port_num;
  int world_fd;
  int amazon_fd;

  int truck_num=5;
  int truck_distance=10;
  

 public:
  Server(const char * myport);
  void run();
  
};