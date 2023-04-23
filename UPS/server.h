
#include "connect_func.h"
#include "connect_world.cpp"


#include "WorldResponseHandler.h"
#include "AmazonResponseHandler.h"

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
  SeqNum seqNum;


 

  void init_world();
  void setup_sockets();
  void init_database();

  void *sendToAmazon();
  void *sendToWorld();
  void *sendAckAmazon();
  void *sendAckWorld();
  void *recvFromAmazon();
  void *recvFromWorld();
  
  static void* recvFromWorldWrapper(void* arg);
  static void* sendAckWorldWrapper(void* arg);
  static void* sendToWorldWrapper(void* arg);
  static void* recvFromAmazonWrapper(void* arg);
  static void* sendAckAmazonWrapper(void* arg);
  static void* sendToAmazonWrapper(void* arg);

 public:

  ThreadSafeQueue<UCommands> world_command;
  ThreadSafeQueue<UACommands> amazon_command;
  ThreadSafeSet world_response;
  ThreadSafeSet amazon_response;
  ThreadSafeQueue<UCommands> world_ack;
  ThreadSafeQueue<UACommands> amazon_ack;


  Server();
  void run();
  ~Server();

};