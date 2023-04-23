#include "threadSafeQueue.h"
#include "threadSafeSet.h"
#include "sql_cmd.h"

class AmazonResponseHandler {
 private: 
    UResponses response;
    ThreadSafeQueue<UCommands> & world_command;
    ThreadSafeSet& world_response;
    ThreadSafeQueue<UCommands> & world_ack;
    int world_id;

 public:
    AmazonResponseHandler(UResponses response, ThreadSafeQueue<UCommands> &world_command, ThreadSafeSet& world_response, ThreadSafeQueue<UCommands> & world_ack, int world_id)
    : response(response), world_command(world_command), world_response(world_response), world_ack(world_ack), world_id(world_id) {}
 

    ~AmazonResponseHandler(){}
};