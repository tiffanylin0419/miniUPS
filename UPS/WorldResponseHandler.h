
#include "proto/ups.pb.h"
#include "proto/ups-amazon.pb.h"

#include "threadSafeMap.h"
#include "threadSafeSet.h"
#include "threadSafeList.h"


class WorldResponseHandler {
 private: 
    UResponses response;
    ThreadSafeMap<UCommands>& world_command;
    ThreadSafeSet& world_response;
    ThreadSafeList & world_ack;

 public:
    WorldResponseHandler(UResponses response, ThreadSafeMap<UCommands>& world_command, ThreadSafeSet& world_response, ThreadSafeList & world_ack)
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
            world_ack.add(response.completions(i).seqnum());
        }
        for(int i=0;i<response.delivered_size();i++){
            //todo
            //if not in world_response
                //deal with response.delivered(i)   
                //add to world_response
            world_ack.add(response.completions(i).seqnum());
        }
    }

    ~WorldResponseHandler(){}
};