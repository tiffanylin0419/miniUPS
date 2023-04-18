#include "server.h"

#define PORT "12345"

int main(){
    const char * port = PORT;
    Server * myServer = new Server(port);
    myServer->run();
    return 0;
}


