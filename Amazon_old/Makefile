CC=g++
CFLAGS=-O3
EXTRAFLAGS=-lpqxx -lpq -lpugixml -lpthread
GPB = `pkg-config --cflags --libs protobuf`

all: main

main: main.cpp server.cpp server.h connect_world.cpp connect_world.h connect_func.cpp connect_func.h head.h
	g++ -o main main.cpp server.cpp connect_world.cpp connect_func.cpp $(TOOLS_CPP) $(PROTO_CC) $(PQXX) $(GDB) $(THREAD) $(GPB) $(VERSION)


clean:
	rm -f *~ *.o test

clobber:
	rm -f *~ *.o

