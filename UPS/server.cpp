#include "server.h"
#include <exception>
#include<thread>
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

string me= "33209";
string Alvin= "32242";
string chimin= "27827";
Server::Server(){
    world_port="12345";
    world_hostname="vcm-33209.vm.duke.edu";
    amazon_port="5688";
    amazon_hostname="vcm-33209.vm.duke.edu";

    sequence_num=0;
    truck_num=5;
    truck_distance=10;
}


int Server::getSeqNum(){
    pthread_mutex_lock(&mutex1);
    int tmp=sequence_num;
    sequence_num++;
    pthread_mutex_unlock(&mutex1);
    return sequence_num;
}

void Server::run() {
    //uncomment
    //init_database();
    init_world();
    

    /*AUCommands aucommand;
    recvMesgFrom<AUCommands>(aucommand, amazon_in);
    cout<<"hi"<<aucommand.acks_size()<<endl;*/
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6;
    pthread_create(&thread1, NULL, &Server::recvFromWorldWrapper,this);
    pthread_create(&thread2, NULL, &Server::sendAckWorldWrapper,this);
    pthread_create(&thread3, NULL, &Server::sendToWorldWrapper,this);
    pthread_create(&thread4, NULL, &Server::recvFromAmazonWrapper,this);
    pthread_create(&thread5, NULL, &Server::sendAckAmazonWrapper,this);
    pthread_create(&thread6, NULL, &Server::sendToAmazonWrapper,this);

    //pthread_detach(thread1);
    // pthread_detach(thread2);
    // pthread_detach(thread3);
    //pthread_detach(thread4);
    // pthread_detach(thread5);
    // pthread_detach(thread6);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
}

void Server::init_database(){
    connection *C;
    try{ 
        //docker
        //C = new connection("dbname=postgres user=postgres password=passw0rd host=db port=5432");
        //no docker
        C = new connection("dbname=ups user=postgres password=Andy860812! hostaddr=127.0.0.1 port=5432");//?
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
        cerr<< "2 Err: recv from world failure"<<endl;
    }else{
        world_id=auInitConnect.worldid();
        cout<<"2 receive world_id = "<<world_id<<endl;
    }
    AUcreate_world_sql(world_id);
    //3 UConnect
    UConnect connect;
    connect.set_worldid(world_id);
    for(int i=0;i<truck_num;i++){
        UInitTruck *truck=connect.add_trucks();
        truck->set_id(i+1);
        truck->set_x(truck_distance*i);
        truck->set_y(truck_distance*i);
        Ucreate_truck_sql( world_id,truck->id(), truck->x(), truck->y());
    }
    connect.set_isamazon(false);
    if(!sendMesgTo<UConnect>(connect, world_out)){
        cerr<< "3 Err: send to world failure"<<endl;
    }else{
        cout<<"3 connect world"<<endl;
    }
    //4 UConnected
    UConnected connected;
    if(!recvMesgFrom<UConnected>(connected, world_in)){
        cout<< "4 Err: recv from world failure"<<endl;
    }else{
        cout<<"4 "<<connected.result()<<endl;
    }
    //5 UAConfirmConnected
    UAConfirmConnected uaConfirmConnected;
    uaConfirmConnected.set_worldid(world_id);
    uaConfirmConnected.set_connected(true);
    if(!sendMesgTo<UAConfirmConnected>(uaConfirmConnected, amazon_out)){
        cerr<< "5 Err: send to amazon failure"<<endl;
    }else{
        cout<<"5 confirmed connect"<<endl;
    }
    
}
    
Server::~Server() {
    close(world_fd);
    close(amazon_fd);
}

/*----------------------------------Send & Recv-----------------------------------*/
void *Server::sendToAmazon(){
    while(true){
        UACommands command=amazon_command.getOne();
        if(!sendMesgTo<UACommands>(command, amazon_out)){
            cerr<<"Error: send to amazon fail"<<endl;
        }else{
            cout<<"send to amazon succeed\n";
        }
        
    }
}

void *Server::sendToWorld(){
    while(true){
        UCommands command=world_command.getOne();
        if(!sendMesgTo<UCommands>(command, world_out)){
            cerr<<"Error: send to world fail"<<endl;
        }else{
            cout<<"send to world succeed\n";
        }
    }
}

void *Server::sendAckAmazon(){
    while(true){
        UACommands command=amazon_ack.getAndRemove();
        if(!sendMesgTo<UACommands>(command, amazon_out)){
            cerr<<"Error: send ack to amazon fail"<<endl;
        }else{
            cout<<"send ack to amazon succeed\n";
        }
    }
}

void *Server::sendAckWorld(){
    while(true){
        UCommands command=world_ack.getAndRemove();
        if(!sendMesgTo<UCommands>(command, world_out)){
            cerr<<"Error: send ack to world fail"<<endl;
        }else{
            cout<<"send ack to world succeed\n";
        }
    }
}

void *Server::recvFromAmazon(){
    while(true){
        AUCommands response;
        if (!recvMesgFrom<AUCommands>(response, amazon_in)) {
            cerr<<"Error: recv from amazon fail"<<endl;
        }else{
            cout<<"recv from amazon succeed\n";
        }
        cout<<"188: "<<response.acks_size()<<endl;
        AmazonResponseHandler h(response, world_command, amazon_command, amazon_response, amazon_ack, world_id);
        /*pthread_t thread;
        pthread_create(&thread, NULL, h.handleWrapper,&h);
        pthread_detach(thread);*/
        h.handle();
    }
}

void *Server::recvFromWorld(){
    while(true){
        UResponses response;
        if (!recvMesgFrom<UResponses>(response, world_in)) {
            cerr<<"Error: recv from world fail"<<endl;
        }else{
            cout<<"recv from world succeed\n";
        }
        WorldResponseHandler h(response, amazon_command, world_command, world_response, world_ack, world_id);
        /*pthread_t thread;
        pthread_create(&thread, NULL, h.handleWrapper,&h);
        pthread_detach(thread);*/
        h.handle();
    }
}


/*----------------------------------Send & Recv Wrapper-----------------------------------*/
void* Server::recvFromWorldWrapper(void* arg) {
    Server* server = static_cast<Server*>(arg);
    return server->recvFromWorld();
}

void* Server::sendAckWorldWrapper(void* arg) {
    Server* server = static_cast<Server*>(arg);
    return server->sendAckWorld();
}

void* Server::sendToWorldWrapper(void* arg) {
    Server* server = static_cast<Server*>(arg);
    return server->sendToWorld();
}

void* Server::recvFromAmazonWrapper(void* arg) {
    Server* server = static_cast<Server*>(arg);
    return server->recvFromAmazon();
}

void* Server::sendAckAmazonWrapper(void* arg) {
    Server* server = static_cast<Server*>(arg);
    return server->sendAckAmazon();
}

void* Server::sendToAmazonWrapper(void* arg) {
    Server* server = static_cast<Server*>(arg);
    return server->sendToAmazon();
}