#ifndef SEQ_NUM_H
#define SEQ_NUM_H

#include <mutex>

class SeqNum {
private:
    static int seqNum;
    static std::mutex mutex1;

public:

    static int get() {
        std::unique_lock<std::mutex> lock(mutex1);
        seqNum++;
        return seqNum;
    }
};

#endif
