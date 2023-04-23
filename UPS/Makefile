CC=g++
CFLAGS=-O3
EXTRAFLAGS= -lpqxx -lpq -lpugixml -lpthread
GPB = `pkg-config --cflags --libs protobuf`
THREAD_SAFE_H = threadSafe/threadSafeQueue.h threadSafe/threadSafeSet.h 
SQL_H = sql_cmd.h 
SQL_CPP = sql_cmd.cpp 
PROTO_H = proto/ups.pb.h proto/ups-amazon.pb.h
PROTO_CC = proto/ups.pb.cc proto/ups-amazon.pb.cc

all: main

main: main.cpp server.cpp server.h connect_world.cpp connect_world.h connect_func.h head.h WorldResponseHandler.h AmazonResponseHandler.h seqNum.h $(THREAD_SAFE_H) $(SQL_H) $(PROTO_H)
	g++ -o main main.cpp server.cpp connect_world.cpp connect_func.cpp WorldResponseHandler.h AmazonResponseHandler.h $(EXTRAFLAGS) $(PQXX) $(GPB) $(SQL_CPP) $(PROTO_CC)


clean:
	rm -f *~ *.o main

clobber:
	rm -f *~ *.o

