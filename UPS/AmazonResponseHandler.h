#include "threadSafe/threadSafeQueue.h"
#include "threadSafe/threadSafeSet.h"

#include "sql_cmd.h"
#include "seqNum.h"

class AmazonResponseHandler {
 private: 
    AUCommands response;
    ThreadSafeQueue<UCommands> & world_command;
    ThreadSafeQueue<UACommands> & amazon_command;
    ThreadSafeSet& amazon_response;
    ThreadSafeQueue<UACommands> & amazon_ack;
    int world_id;

 public:
    AmazonResponseHandler(AUCommands response, ThreadSafeQueue<UCommands> &world_command, ThreadSafeQueue<UACommands> &amazon_command, ThreadSafeSet& amazon_response, ThreadSafeQueue<UACommands> & amazon_ack, int world_id)
    : response(response), world_command(world_command), amazon_command(amazon_command), amazon_response(amazon_response), amazon_ack(amazon_ack), world_id(world_id) {}
 
   void handle(){
      for(int i=0;i<response.acks_size();i++){
         amazon_command.remove(response.acks(i));
      }
      for(int i=0;i<response.pickupreq_size();i++){
         AUInitPickUp r=response.pickupreq(i);
         if(!amazon_response.contains(r.seqnum())){
            //todo
            int truck_id=1;//sql
            amazon_response.add(r.seqnum());
            addUGoPickup(r, truck_id);
         }   
         addAmazonAck(r.seqnum());
      }
      for(int i=0;i<response.loaded_size();i++){
         AULoaded r=response.loaded(i);
         if(!amazon_response.contains(r.seqnum())){
            //todo
            //sql
            amazon_response.add(r.seqnum());
            //14
            //add 15 to world_command

         }   
         addAmazonAck(r.seqnum());
      }
      //todo
      //AUPackageResponse
   }

    void addAmazonAck(int seqnum){
         UACommands command;
         command.add_acks(seqnum);
         amazon_ack.add(seqnum, command);
    }

    void addUGoPickup(AUInitPickUp &r, int truckid){
      int whid=r.whid();
      int seqNum=SeqNum::get();
      UCommands command;
      UGoPickup *ugopickup=command.add_pickups();
      ugopickup->set_whid(whid);
      ugopickup->set_seqnum(seqNum);
      world_command.add(seqNum,command);
    }

    ~AmazonResponseHandler(){}
};