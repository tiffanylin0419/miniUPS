#include "threadSafe/threadSafeQueue.h"
#include "threadSafe/threadSafeSet.h"
#include "sql_cmd.h"

class WorldResponseHandler {
 private: 
    UResponses response;
    ThreadSafeQueue<UCommands> & world_command;
    ThreadSafeSet& world_response;
    ThreadSafeQueue<UCommands> & world_ack;
    int world_id;

 public:
    WorldResponseHandler(UResponses response, ThreadSafeQueue<UCommands> &world_command, ThreadSafeSet& world_response, ThreadSafeQueue<UCommands> & world_ack, int world_id)
    : response(response), world_command(world_command), world_response(world_response), world_ack(world_ack), world_id(world_id) {}
 
    void handle(){
        for(int i=0;i<response.acks_size();i++){
            world_command.remove(response.acks(i));
        }
        for(int i=0;i<response.completions_size();i++){
            UFinished r=response.completions(i);
            if(!world_response.contains(r.seqnum())){
                Ufinish_sql(world_id, r.truckid(), r.status(), r.x(), r.y());
                world_response.add(r.seqnum());
                //todo
                //10
                    //add 11 to amazon_command
                //17+
                    //none
            }   
            addWorldAck(r.seqnum());
        }
        for(int i=0;i<response.delivered_size();i++){
            UDeliveryMade r=response.delivered(i);
            if(!world_response.contains(r.seqnum())){
                UDeliveryMade_sql(world_id, r.truckid(), r.packageid());
                world_response.add(r.seqnum());
                //todo
                //16
                //add 17 to amazon_command
            }
            addWorldAck(r.seqnum());
        }
        // for(int i=0;i<response.truckstatus_size();i++){
        //     //todo
        //     UTruck r=response.truckstatus(i);
        //     if(!world_response.contains(r.seqnum())){
        //         UTruck_sql(world_id, r.truckid(), r.status(), r.x(), r.y());
        //         world_response.add(r.seqnum());
        //     }
        //     world_ack.add(r.seqnum());
        // }
    }

    void addWorldAck(int seqnum){
        UCommands command;
        command.add_acks(seqnum);
        world_ack.add(seqnum, command);
    }

    ~WorldResponseHandler(){}
};