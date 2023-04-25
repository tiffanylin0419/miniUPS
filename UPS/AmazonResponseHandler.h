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
      //8 AUInitPickup
      for(int i=0;i<response.pickupreq_size();i++){
         AUInitPickUp r=response.pickupreq(i);
         if(!amazon_response.contains(r.seqnum())){
            string description="";
            for(int j=0;j<r.product_size();j++){
               AUProduct product=r.product(j);
               description.append(product.description()+" * "+to_string(product.count())+", ");
            }
            int truck_id=AUInitPickUp_sql(world_id, r.whid(), r.accountname(), r.deliverylocation().packageid(), r.deliverylocation().x(), r.deliverylocation().y(), description);
            if(truck_id!=-1){
               truck_id=AUInitPickUp_sql(world_id, r.whid(), r.accountname(), r.deliverylocation().packageid(), r.deliverylocation().x(), r.deliverylocation().y(), description);
            }
            amazon_response.add(r.seqnum());
            addUGoPickup(r, truck_id);
         }   
         addAmazonAck(r.seqnum());
      }
      // 14 AULoaded
      for(int i=0;i<response.loaded_size();i++){
         AULoaded r=response.loaded(i);
         if(!amazon_response.contains(r.seqnum())){
            //todo
            //r.shipid()
            //from package table find package_id, des_x, des_y, 
            //send truckid, UDeliveryLocation:packages(packageid, x,y)
            UGoDeliver ugodeliver;//sql return a UGoDeliver without seq
            amazon_response.add(r.seqnum());
            addUGoDeliver(ugodeliver);
         }   
         addAmazonAck(r.seqnum());
      }
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

   void addUGoDeliver(UGoDeliver &ugodeliver){
      UCommands command;
      UGoDeliver *ugodeliver2=command.add_deliveries();
      *ugodeliver2=ugodeliver;
      int seqNum=SeqNum::get();
      ugodeliver2->set_seqnum(seqNum);
      world_command.add(seqNum,command);
   }

   ~AmazonResponseHandler(){}
};