#include "threadSafe/threadSafeQueue.h"
#include "threadSafe/threadSafeSet.h"
#include "sql_cmd.h"

#include "seqNum.h"

class WorldResponseHandler {
 private: 
    UResponses response;
    ThreadSafeQueue<UACommands> &amazon_command;
    ThreadSafeQueue<UCommands> & world_command;
    ThreadSafeSet& world_response;
    ThreadSafeQueue<UCommands> & world_ack;
    int world_id;

 public:
    WorldResponseHandler(UResponses response, ThreadSafeQueue<UACommands> &amazon_command, ThreadSafeQueue<UCommands> &world_command, ThreadSafeSet& world_response, ThreadSafeQueue<UCommands> & world_ack, int world_id)
    : response(response), amazon_command(amazon_command), world_command(world_command), world_response(world_response), world_ack(world_ack), world_id(world_id){}
 
    void handle(){
        for(int i=0;i<response.acks_size();i++){
            world_command.remove(response.acks(i));
        }
        for(int i=0;i<response.completions_size();i++){
            UFinished r=response.completions(i);
            if(!world_response.contains(r.seqnum())){
                Ufinish_sql(world_id, r.truckid(), r.status(), r.x(), r.y());
                world_response.add(r.seqnum());
                addUATruckArrived(r);
            }   
            addWorldAck(r.seqnum());
        }
        for(int i=0;i<response.delivered_size();i++){
            UDeliveryMade r=response.delivered(i);
            if(!world_response.contains(r.seqnum())){
                UDeliveryMade_sql(world_id, r.truckid(), r.packageid());
                world_response.add(r.seqnum());
                addUADelievered(r);
            }
            addWorldAck(r.seqnum());
        }
    }

    void addWorldAck(int seqnum){
        UCommands command;
        command.add_acks(seqnum);
        world_ack.add(seqnum, command);
    }

    void addUATruckArrived(UFinished &r){
        //todo
        //if status = arrive warehouse
            //從package table找到所有用這台truck且 loading的packageid
    }

    void addUADelievered(UDeliveryMade &r){
        int truckid=r.truckid();
        int packageid=r.packageid();
        int seqNum=SeqNum::get();
        UACommands command;
        UADelivered *uadelivered=command.add_delivered();
        uadelivered->set_truckid(truckid);
        uadelivered->set_packageid(packageid);
        uadelivered->set_seqnum(seqNum);
        amazon_command.add(seqNum,command);
    }

    ~WorldResponseHandler(){}
};