CC=g++
CFLAGS=-O3
EXTRAFLAGS=-lpqxx -lpq -lpugixml -lpthread
GPB = `pkg-config --cflags --libs protobuf`

all: main

main: main.cpp connect_world.cpp connect_world.h connect_func.cpp connect_func.h head.h proto/ups.pb.cc
	g++ -o main main.cpp connect_world.cpp connect_world.h connect_func.cpp connect_func.h head.h proto/ups.pb.cc	$(TOOLS_CPP) $(PROTO_CC) $(PQXX) $(GDB) $(THREAD) $(GPB) $(VERSION)


clean:
	rm -f *~ *.o test

clobber:
	rm -f *~ *.o

