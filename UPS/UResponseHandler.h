
#include "proto/ups.pb.h"
#include "proto/ups-amazon.pb.h"

#include "threadSafeMap.h"
#include "threadSafeSet.h"
#include "threadSafeList.h"


class UResponseHandler {
 private: 
    UResponses response;
    ThreadSafeMap<UCommands>& world_command;
    ThreadSafeSet& world_response;
    ThreadSafeList<UCommands>& world_ack;

 public:
    UResponseHandler(UResponses response, ThreadSafeMap<UCommands>& world_command, ThreadSafeSet& world_response, ThreadSafeList<UCommands>& world_ack)
    : response(response), world_command(world_command), world_response(world_response), world_ack(world_ack) {}
 

    void handle(){
        for(int i=0;i<response.acks_size();i++){
            world_command.remove(response.acks(i));
        }
        for(int i=0;i<response.completions_size();i++){
            //todo
            //if not in world_response
                //deal with response.completions(i)
                //add to world_response
            //add to world_ack
        }
        for(int i=0;i<response.delivered_size();i++){
            //todo
            //if not in world_response
                //deal with response.delivered(i)   
                //add to world_response
            //add to world_ack
        }

    }

    ~UResponseHandler(){}
};