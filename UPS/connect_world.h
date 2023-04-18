#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "connect_func.h"
#include "proto/ups.pb.h"

//this is adapated from code that a Google engineer posted online 
template<typename T>
bool sendMesgTo(const T & message, google::protobuf::io::FileOutputStream *out) ;

//this is adapated from code that a Google engineer posted online 
template<typename T>
bool recvMesgFrom(T & message, google::protobuf::io::FileInputStream * in );
