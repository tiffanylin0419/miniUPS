#include "server.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


void Server::run() {
    this->world_fd = setup_client("vcm-33209.vm.duke.edu", this->port_num);
    int temp_fd = setup_server("5688");
    if (temp_fd == -1) {
        return;
    }
    int client_fd = accept_server(temp_fd);
    if (client_fd == -1) {
      cout << "(no-id): ERROR in connecting client" << endl;
    }
    


    close(temp_fd);
    close(world_fd);
}




    