
#include "proto/ups.pb.h"
#include "proto/ups-amazon.pb.h"

#include "threadSafeMap.h"
#include "threadSafeSet.h"
#include "threadSafeList.h"
#include "sql_cmd.h"

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
            UFinished r=response.completions(i);
            if(!world_response.contains(r.seqnum())){
                //sql deal with r
                world_response.add(r.seqnum());
                //10
                    //add 11 to amazon_command
                //17+
                    //none
            }   
            world_ack.add(r.seqnum());
        }
        for(int i=0;i<response.delivered_size();i++){
            //todo
            UDeliveryMade r=response.delivered(i);
            if(!world_response.contains(r.seqnum())){
                //sql deal with r
                world_response.add(r.seqnum());
                //16
                //add 17 to amazon_command
            }
            world_ack.add(r.seqnum());
        }
    }

    ~WorldResponseHandler(){}
};