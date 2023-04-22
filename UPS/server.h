
#include "connect_func.h"
#include "connect_world.cpp"



#include "WorldResponseHandler.h"

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
  int getSeqNum();

  void *sendToAmazon();
  void *sendToWorld();
  void *sendAckAmazon();
  void *sendAckWorld();
  void *recvFromAmazon();
  void *recvFromWorld();
  
  static void* recvFromWorldWrapper(void* arg);

 public:

  ThreadSafeQueue<UCommands> world_command;
  ThreadSafeQueue<AUCommands> amazon_command;
  ThreadSafeSet world_response;
  ThreadSafeSet amazon_response;
  ThreadSafeList world_ack;
  ThreadSafeList amazon_ack;


  Server();
  void run();
  ~Server();

};