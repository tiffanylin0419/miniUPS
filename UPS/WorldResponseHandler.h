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
    int world_id;

 public:
    WorldResponseHandler(UResponses response, ThreadSafeMap<UCommands>& world_command, ThreadSafeSet& world_response, ThreadSafeList & world_ack, int world_id)
    : response(response), world_command(world_command), world_response(world_response), world_ack(world_ack), world_id(world_id) {}
 
    void handle(){
        for(int i=0;i<response.acks_size();i++){
            world_command.remove(response.acks(i));
        }
        for(int i=0;i<response.completions_size();i++){
            //todo
            UFinished r=response.completions(i);
            if(!world_response.contains(r.seqnum())){
                //Ufinish_sql(int truck_id, std::string truck_status, int new_x, int new_y);
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
                UDeliveryMade_sql(world_id, r.truckid(), r.packageid());
                world_response.add(r.seqnum());
                //16
                //add 17 to amazon_command
            }
            world_ack.add(r.seqnum());
        }
    }

    ~WorldResponseHandler(){}
};