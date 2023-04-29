#include "server.h"


std::mutex SeqNum::mutex1;
int SeqNum::seqNum = 0;

int main(){
    Server * myServer = new Server();
    myServer->run();
    return 0;
}


