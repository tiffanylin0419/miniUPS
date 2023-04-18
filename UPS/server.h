#include "proto/ups.pb.h"
#include "connect_func.h"
#include "connect_world.cpp"


class Server {
 private:
  const char * port_num;
  int world_fd;
  int amazon_fd;
  

 public:
  Server(const char * myport) : port_num(myport) {}
  void run();
  
};