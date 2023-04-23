#include "threadSafe/threadSafeQueue.h"
#include "threadSafe/threadSafeSet.h"
#include "sql_cmd.h"

class AmazonResponseHandler {
 private: 
    AUCommands response;
    ThreadSafeQueue<UACommands> & amazon_command;
    ThreadSafeSet& amazon_response;
    ThreadSafeQueue<UACommands> & amazon_ack;
    int world_id;

 public:
    AmazonResponseHandler(AUCommands response, ThreadSafeQueue<UACommands> &amazon_command, ThreadSafeSet& amazon_response, ThreadSafeQueue<UACommands> & amazon_ack, int world_id)
    : response(response), amazon_command(amazon_command), amazon_response(amazon_response), amazon_ack(amazon_ack), world_id(world_id) {}
 

    ~AmazonResponseHandler(){}
};