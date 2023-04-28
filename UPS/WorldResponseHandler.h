#include <pqxx/pqxx>

#include "threadSafe/threadSafeQueue.h"
#include "threadSafe/threadSafeSet.h"
#include "sql_cmd.h"

#include "seqNum.h"

using namespace pqxx;

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
 
    void* handle(){
        for(int i=0;i<response.acks_size();i++){
            world_command.remove(response.acks(i));
        }
        // 10 Truck ready & 17 complete
        for(int i=0;i<response.completions_size();i++){
            UFinished r=response.completions(i);
            if(!world_response.contains(r.seqnum())){
                world_response.add(r.seqnum());
                addUATruckArrived(r);
            }   
            addWorldAck(r.seqnum());
            cout<<"add world_ack = "+to_string(r.seqnum());
        }
        // 16 Delivered
        for(int i=0;i<response.delivered_size();i++){
            UDeliveryMade r=response.delivered(i);
            if(!world_response.contains(r.seqnum())){
                UDeliveryMade_sql(world_id, r.truckid(), r.packageid());
                world_response.add(r.seqnum());
                addUADelievered(r);
            }
            addWorldAck(r.seqnum());
        }
        return NULL;
    }

    static void* handleWrapper(void* arg) {
        WorldResponseHandler* h = static_cast<WorldResponseHandler*>(arg);
        return h->handle();
    }

    void addWorldAck(int seqnum){
        cout<<"add world_azk = "+to_string(seqnum);
        UCommands command;
        command.add_acks(seqnum);
        world_ack.add(seqnum, command);
    }

    void addUATruckArrived(UFinished &r){
        result R=Ufinish_sql(world_id, r.truckid(), r.status(), r.x(), r.y());
        for (result::const_iterator c = R.begin(); c != R.end(); ++c){
            UACommands command;
            UATruckArrived *uatruckarrived=command.add_truckarrived();
            int seqNum=SeqNum::get();
            uatruckarrived->set_truckid(r.truckid());
            uatruckarrived->set_shipid(c[0].as<int>());
            uatruckarrived->set_seqnum(seqNum);
            amazon_command.add(seqNum,command);
        }
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