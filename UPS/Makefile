CC=g++
CFLAGS=-O3
EXTRAFLAGS= -lpqxx -lpq -lpugixml -lpthread
GPB = `pkg-config --cflags --libs protobuf`
THREADSAFE_H = threadSafeQueue.h threadSafeSet.h
THREADSAFE_CPP = threadSafeQueue.cpp threadSafeSet.cpp
all: main

main: main.cpp server.cpp server.h connect_world.cpp connect_world.h connect_func.cpp connect_func.h head.h proto/ups.pb.cc proto/ups-amazon.pb.cc WorldResponseHandler.h AmazonResponseHandler.h sql_cmd.h sql_cmd.cpp  $(THREADSAFE_H)
	g++ -o main main.cpp server.cpp connect_world.cpp connect_func.cpp proto/ups.pb.cc proto/ups-amazon.pb.cc WorldResponseHandler.h AmazonResponseHandler.h sql_cmd.cpp $(EXTRAFLAGS) $(TOOLS_CPP) $(PROTO_CC) $(PQXX) $(GDB) $(GPB) $(VERSION) $(THREADSAFE_H) $(THREADSAFE_CPP)


clean:
	rm -f *~ *.o main

clobber:
	rm -f *~ *.o

