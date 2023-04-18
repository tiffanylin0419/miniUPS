#include "server.h"

#define PORT "23456"

int main(){
    const char * port = PORT;
    Server * myServer = new Server(port);
    myServer->run();
    return 0;
}


