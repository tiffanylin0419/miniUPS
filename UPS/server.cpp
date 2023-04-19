#include "server.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;



Server::Server(){
    world_port="12345";
    world_hostname="vcm-33209.vm.duke.edu";
    amazon_port="11111";
    amazon_hostname="vcm-33209.vm.duke.edu";

    sequence_num=0;

    truck_num=5;
    truck_distance=10;
    //connect DB

}

void Server::run() {
    
    init_world();

    //use handle or while loop to keep getting and sending data?

    //other groups vcm
    amazon_fd = setup_client(amazon_hostname, amazon_port);
    google::protobuf::io::FileInputStream * amazon_in=new google::protobuf::io::FileInputStream(amazon_fd);
    google::protobuf::io::FileOutputStream * amazon_out=new google::protobuf::io::FileOutputStream(amazon_fd);


    
}

void Server::init_database(){
    connection *C;
    /*try{ 
        //docker
        C = new connection("dbname=postgres user=postgres password=passw0rd host=db port=5432");
        //no docker
        //C = new connection("dbname=EXCHANGE_SERVER user=postgres password=passw0rd");
        if (C->is_open()) {
        } else {
        cout << "Can't open database" << endl;
        return NULL;
        } 
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return NULL;
    }
    string response=requestToResponse(C, request);
    C->disconnect();*/
}


void Server::init_world(){
    world_fd = setup_client(world_hostname, world_port);
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

    if(!sendMesgTo<UConnect>(connect, world_out)){
        cout<< "send to world failure";
    }

    UConnected connected;
    if(!recvMesgFrom<UConnected>(connected, world_in)){
        cout<< "recv from world failure";
    }
    cout<<connected.result()<<endl;
    cout<<connected.worldid()<<endl;
}

    
Server::~Server() {
    close(world_fd);
    close(amazon_fd);
}