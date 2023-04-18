#include "connect_world.h"


int main(){
    int server_fd = setup_client("vcm-33209.vm.duke.edu", "12345");
    google::protobuf::io::FileInputStream * in=new google::protobuf::io::FileInputStream(server_fd);
    google::protobuf::io::FileOutputStream *out=new google::protobuf::io::FileOutputStream(server_fd);

    UConnect connect;
    connect.set_worldid(12345);
    UInitTruck *truck=connect.add_trucks();
    truck->set_id(1);
    truck->set_x(2);
    truck->set_y(3);
    connect.set_isamazon(false);

    return 0;
}