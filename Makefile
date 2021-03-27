CC=g++
CFLAGS=-O3
EXTRAFLAGS=-lpqxx -lpq

all: server

server: server.cpp database.cpp
				$(CC) $(CFLAGS) -o server server.cpp database.cpp $(EXTRAFLAGS)

clean:
				rm -f *~ *.o server

clobber:
				rm -f *~ *.o