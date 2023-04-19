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
}

void Server::run() {
    //uncomment
    //init_database();
    init_world();
    
    //todo
    //use handle or while loop to keep getting and sending data?
    
}

void Server::init_database(){
    connection *C;
    try{ 
        //docker
        //C = new connection("dbname=postgres user=postgres password=passw0rd host=db port=5432");
        //no docker
        C = new connection("dbname=EXCHANGE_SERVER user=postgres password=passw0rd");//?
        if (C->is_open()) {
        } else {
        cout << "Can't open database" << endl;
        return;
        } 
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return;
    }
    C->disconnect();
}

void Server::setup_sockets(){
    amazon_fd = setup_client(amazon_hostname, amazon_port);
    amazon_in=new google::protobuf::io::FileInputStream(amazon_fd);
    amazon_out=new google::protobuf::io::FileOutputStream(amazon_fd);

    world_fd = setup_client(world_hostname, world_port);
    world_in=new google::protobuf::io::FileInputStream(world_fd);
    world_out=new google::protobuf::io::FileOutputStream(world_fd);
}


void Server::init_world(){
    setup_sockets();
    //2 AUInitConnect
    AUInitConnect auInitConnect;
    if(!recvMesgFrom<AUInitConnect>(auInitConnect, amazon_in)){
        cerr<< "2 recv from world failure"<<endl;
    }
    world_id-=auInitConnect.worldid();
    cout<<"2 receive world_id = "<<world_id<<endl;
    //3 UConnect
    UConnect connect;
    connect.set_worldid(world_id);
    for(int i=0;i<truck_num;i++){
        UInitTruck *truck=connect.add_trucks();
        truck->set_id(i+1);
        truck->set_x(truck_distance*i);
        truck->set_y(truck_distance*i);
    }
    connect.set_isamazon(false);
    if(!sendMesgTo<UConnect>(connect, world_out)){
        cerr<< "3 send to world failure"<<endl;
    }
    cout<<"3 connect world"<<endl;
    //4 UConnected
    UConnected connected;
    if(!recvMesgFrom<UConnected>(connected, world_in)){
        cout<< "4 recv from world failure"<<endl;
    }
    cout<<"4 "<<connected.result()<<endl;
    //5 UAConfirmConnected
    UAConfirmConnected uaConfirmConnected;
    uaConfirmConnected.set_worldid(world_id);
    uaConfirmConnected.set_connected(true);
    uaConfirmConnected.set_seqnum(getSeqNum());
    if(!sendMesgTo<UAConfirmConnected>(uaConfirmConnected, amazon_out)){
        cerr<< "5 send to amazon failure"<<endl;
    }
    cout<<"5 confirmed connect"<<endl;
}


int Server::getSeqNum(){
    pthread_mutex_lock(&mutex1);
    int tmp=sequence_num;
    sequence_num++;
    pthread_mutex_unlock(&mutex1);
    return sequence_num;
}
    
Server::~Server() {
    close(world_fd);
    close(amazon_fd);
}