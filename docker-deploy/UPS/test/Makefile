CC=g++
CFLAGS=-O3
EXTRAFLAGS= -lpqxx -lpq -lpugixml -lpthread
GPB = `pkg-config --cflags --libs protobuf`


all: main

main: main.cpp seq.h $(THREAD_SAFE_H) $(SQL_H) $(PROTO_H)
	g++ -o main main.cpp $(EXTRAFLAGS) $(PQXX) $(GPB) $(SQL_CPP) $(PROTO_CC)


clean:
	rm -f *~ *.o main

clobber:
	rm -f *~ *.o

