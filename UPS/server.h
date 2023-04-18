#include "proto/ups.pb.h"
#include "connect_func.h"
#include "connect_world.cpp"


class Server {
 private:
  const char * world_port;
  const char * world_hostname;
  int world_fd;
  const char * amazon_port;
  const char * amazon_hostname;
  int amazon_fd;

  int truck_num;
  int truck_distance;

  void init_world();
  

 public:
  Server();
  void run();
  
};