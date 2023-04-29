#include<cstdio>
#include<cstdlib>
#include <iostream>
#include "seq.h"

std::mutex SeqNum::mutex1;
int SeqNum::seqNum = 0;

int main(){
    std::cout<<"k";
    
    std::cout<<SeqNum::get();
    std::cout<<SeqNum::get();
    return 0;
}