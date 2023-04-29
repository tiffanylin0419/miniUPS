#include "server.h"


std::mutex SeqNum::mutex1;
int SeqNum::seqNum = 0;

int main(){
    cout<<"start running\n";
    Server * myServer = new Server();
    myServer->run();
    return 0;
}


