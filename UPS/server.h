#include "proto/ups.pb.h"
#include "proto/ups-amazon.pb.h"
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
  google::protobuf::io::FileInputStream * amazon_in;
  google::protobuf::io::FileOutputStream * amazon_out;
  google::protobuf::io::FileInputStream * world_in;
  google::protobuf::io::FileOutputStream * world_out;

  int world_id;
  int sequence_num;
  int truck_num;
  int truck_distance;

  void init_world();
  void setup_sockets();
  void init_database();
  
  

 public:
  Server();
  void run();
  ~Server();
};