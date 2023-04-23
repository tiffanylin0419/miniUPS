#ifndef SEQ_NUM_H
#define SEQ_NUM_H

#include <mutex>

class SeqNum{
  private: 
    int seqNum=0;
    std::mutex mutex1;

 public:
    SeqNum() {}
    int get(){
        std::unique_lock<std::mutex> lock(mutex1);
        seqNum++;
        return seqNum;
    }

    ~SeqNum(){}
};


#endif 