#include "server.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;



Server::Server(const char * myport) : port_num(myport) {
    //connect DB
}

void Server::run() {
    world_fd = setup_client("vcm-33209.vm.duke.edu", port_num);
    google::protobuf::io::FileInputStream * world_in=new google::protobuf::io::FileInputStream(world_fd);
    google::protobuf::io::FileOutputStream * world_out=new google::protobuf::io::FileOutputStream(world_fd);

    UConnect connect;
    //connect.set_worldid(12345);
    for(int i=0;i<truck_num;i++){
        UInitTruck *truck=connect.add_trucks();
        truck->set_id(i+1);//1~truck_num
        truck->set_x(truck_distance*i);
        truck->set_y(truck_distance*i);
    }
    connect.set_isamazon(false);

    bool b=sendMesgTo<UConnect>(connect, world_out) ;
    if(!b){
        cout<< "send to world failure";
    }

    UConnected connected;
    b=recvMesgFrom<UConnected>(connected, world_in) ;
    if(!b){
        cout<< "recv from world failure";
    }
    cout<<connected.result()<<endl;
    cout<<connected.worldid()<<endl;


    //other groups vcm
    amazon_fd = setup_client("vcm-33209.vm.duke.edu", "11111");
    google::protobuf::io::FileInputStream * amazon_in=new google::protobuf::io::FileInputStream(amazon_fd);
    google::protobuf::io::FileOutputStream * amazon_out=new google::protobuf::io::FileOutputStream(amazon_fd);


    close(world_fd);
    close(amazon_fd);
}




    