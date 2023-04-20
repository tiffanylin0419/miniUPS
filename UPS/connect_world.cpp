#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "connect_func.h"
#include "proto/ups.pb.h"

//this is adapated from code that a Google engineer posted online 
template<typename T>
bool sendMesgTo(const T & message, google::protobuf::io::FileOutputStream *out) { 
    {   //extra scope: make output go away before out->Flush()
        // We create a new coded stream for each message.
        // Don’t worry, this is fast. 
        google::protobuf::io::CodedOutputStream output(out); 
        // Write the size.
        const int size = message.ByteSize(); output.WriteVarint32(size);
        uint8_t* buffer=output.GetDirectBufferForNBytesAndAdvance(size); 
        if (buffer != NULL) {
            // Optimization:The message fits in one buffer, so use 
            // the faster direct-to-array serialization path. 
            message.SerializeWithCachedSizesToArray(buffer);
        } else {
            // Slightly-slower path when message is multiple buffers
            message.SerializeWithCachedSizes(&output);
            if (output.HadError()) {
                return false; 
            }
        } 
    }
    out->Flush();
    return true; 
}

//this is adapated from code that a Google engineer posted online 
template<typename T>
bool recvMesgFrom(T & message, google::protobuf::io::FileInputStream * in ){ 
    google::protobuf::io::CodedInputStream input(in);
    uint32_t size;
    if (!input.ReadVarint32(&size)) {
        return false; 
    }
    // Tell the stream not to read beyond that size. 
    google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(size);
    // Parse the message.
    if (!message.MergeFromCodedStream(&input)) {
        return false; 
    }
    if (!input.ConsumedEntireMessage()) { 
        return false;
    }
    // Release the limit. 
    input.PopLimit(limit); 
    return true;
}

int main(){
    int server_fd = setup_client("vcm-32227.vm.duke.edu", "12345");
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